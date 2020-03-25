
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#ifdef RCT_DEV

#include "websocketexecutor.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QQueue>
#include <QTimer>
#include <QWebSocket>

namespace {
const int CHROME_DEV_TOOL_AWAIT_TIMEOUT = 5000;
}

class WebSocketExecutorPrivate : public QObject {
public:
    WebSocketExecutorPrivate(WebSocketExecutor* e) : q_ptr(e) {}

    bool processRequests();
    void processRequest(const QByteArray& request,
                        const IJsExecutor::ExecuteCallback& callback = IJsExecutor::ExecuteCallback());
    void setupWebSocket(const QUrl& url);
    QByteArray getMessageData(const QVariantMap& messageData);

public slots:
    void readReply(const QString& reply);
    void passReceivedDataToCallback(const QByteArray& data);

public:
    QQueue<QByteArray> m_requestQueue;
    QQueue<IJsExecutor::ExecuteCallback> m_responseQueue;
    QByteArray m_inputBuffer;
    WebSocketExecutor* q_ptr = nullptr;
    QWebSocket m_webSocket;
    QVariantMap m_injectedObjects;
    bool m_readyToSendAppScriptData = false;
    QByteArray m_applicationScriptData;
};

WebSocketExecutor::WebSocketExecutor(const QUrl& url, QObject* parent)
    : IJsExecutor(parent), d_ptr(new WebSocketExecutorPrivate(this)) {
    Q_D(WebSocketExecutor);

    d->setupWebSocket(url);
}

WebSocketExecutor::~WebSocketExecutor() {}

void WebSocketExecutorPrivate::setupWebSocket(const QUrl& url) {

    connect(&m_webSocket, &QWebSocket::connected, [=]() {
        QUrl startDevToolsURL = url.resolved(QStringLiteral("launch-js-devtools"));
        startDevToolsURL.setScheme("http");
        QNetworkAccessManager* networkAccessManager = new QNetworkAccessManager();
        QNetworkReply* reply = networkAccessManager->get(QNetworkRequest(startDevToolsURL));

        connect(reply, &QNetworkReply::readyRead, [=]() {
            if (reply->readAll() == QStringLiteral("OK")) {
                QTimer* timer = new QTimer;
                timer->setSingleShot(true);
                timer->setInterval(CHROME_DEV_TOOL_AWAIT_TIMEOUT);

                QObject::connect(timer, &QTimer::timeout, [=]() {
                    QByteArray data = getMessageData(QVariantMap{{"method", "prepareJSRuntime"}});
                    processRequest(data, [=](const QJsonDocument&) {
                        m_readyToSendAppScriptData = true;
                        q_ptr->executeApplicationScriptOnSocketReady();
                    });
                    timer->deleteLater();
                });
                timer->start();
            }
            reply->abort();
        });

        connect(reply, &QNetworkReply::finished, [=]() {
            reply->deleteLater();
            networkAccessManager->deleteLater();
        });
    });

    connect(&m_webSocket,
            static_cast<void (QWebSocket::*)(QAbstractSocket::SocketError)>(&QWebSocket::error),
            [=](QAbstractSocket::SocketError) { qDebug() << m_webSocket.errorString(); });

    connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &WebSocketExecutorPrivate::readReply);

    m_webSocket.open(url);
}

void WebSocketExecutor::injectJson(const QString& name, const QVariant& data) {
    QJsonDocument doc = QJsonDocument::fromVariant(data);
    d_ptr->m_injectedObjects[name] = doc.toJson(QJsonDocument::Compact);
}

void WebSocketExecutor::executeApplicationScript(const QByteArray& script, const QUrl& sourceUrl) {
    QVariantMap message = QVariantMap{
        {"method", "executeApplicationScript"}, {"url", sourceUrl.toString()}, {"inject", d_ptr->m_injectedObjects}};
    d_ptr->m_applicationScriptData = d_ptr->getMessageData(message);
    executeApplicationScriptOnSocketReady();
}

void WebSocketExecutor::executeJSCall(const QString& method,
                                      const QVariantList& args,
                                      const IJsExecutor::ExecuteCallback& callback) {

    QVariantMap message = QVariantMap{{"method", method}, {"arguments", args}};
    QByteArray data = d_ptr->getMessageData(message);

    d_ptr->processRequest(data, callback);
}

void WebSocketExecutor::init() {}

QByteArray WebSocketExecutorPrivate::getMessageData(const QVariantMap& messageData) {
    static int lastId = 10000;

    QVariantMap data = messageData;
    data["id"] = lastId++;
    QJsonDocument doc = QJsonDocument::fromVariant(data);

    return doc.toJson(QJsonDocument::Compact);
}

void WebSocketExecutor::executeApplicationScriptOnSocketReady() {
    // JS source code should be loaded from bundler and WebSocket should be ready
    // to send requests to JS side
    if (d_ptr->m_readyToSendAppScriptData && !d_ptr->m_applicationScriptData.isEmpty()) {
        d_ptr->processRequest(d_ptr->m_applicationScriptData,
                              [=](const QJsonDocument&) { Q_EMIT applicationScriptDone(); });
        d_ptr->m_applicationScriptData.clear();
    }
}

bool WebSocketExecutorPrivate::processRequests() {
    if (m_webSocket.state() != QAbstractSocket::ConnectedState || m_requestQueue.isEmpty()) {
        return false;
    }

    QByteArray request = m_requestQueue.dequeue();
    m_webSocket.sendTextMessage(request);
    m_webSocket.flush();
    return true;
}

void WebSocketExecutorPrivate::readReply(const QString& reply) {
    passReceivedDataToCallback(reply.toLatin1());
}

void WebSocketExecutorPrivate::passReceivedDataToCallback(const QByteArray& data) {
    if (m_responseQueue.size()) {
        IJsExecutor::ExecuteCallback callback = m_responseQueue.dequeue();
        if (callback) {
            QJsonDocument doc;
            if (data != "undefined") {
                QJsonParseError error;
                doc = QJsonDocument::fromJson(data, &error);
                qDebug() << "Input Json document: " << doc << " Error: " << error.errorString();

                // Try to extract "result" value and pass it to callback
                QJsonValue resultValue = doc.object().value("result");
                if (resultValue.isString()) {
                    QString jsonArrayString = resultValue.toString();
                    doc = QJsonDocument::fromJson(jsonArrayString.toLatin1(), &error);
                }

                qDebug() << "Result Json document: " << doc << " Error: " << error.errorString();
            }
            callback(doc);
        }
    }
}

void WebSocketExecutorPrivate::processRequest(const QByteArray& request, const IJsExecutor::ExecuteCallback& callback) {

    m_requestQueue.enqueue(request);
    m_responseQueue.enqueue(callback);

    while (!m_requestQueue.isEmpty() && !m_responseQueue.isEmpty()) {
        if (!processRequests())
            break;
    }
}

#endif // RCT_DEV
