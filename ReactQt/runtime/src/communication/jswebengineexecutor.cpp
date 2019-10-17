
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include "jswebengineexecutor.h"
#include <QDebug>
#include <QEventLoop>
#include <QFile>
#include <QJSEngine>
#include <QJsonDocument>
#include <QJsonDocument>
#include <QSharedPointer>
#include <QWebChannel>
#include <QWebEnginePage>
#include <QWebEngineSettings>

const QString INIT_CHANNEL_FROM_JS = "new QWebChannel(qt.webChannelTransport, \
 function(channel){ \
 var webobj = channel.objects.webobj; \
 window.foo = webobj;  \
 });";

const int WORLD_ID = 0;

class JSWebEngineExecutorPrivate : public QObject {
public:
    JSWebEngineExecutorPrivate(JSWebEngineExecutor* e) : QObject(e), q_ptr(e) {
        loadEmptyPage();
        // Uncomment when there is a need to setup web channel
        // initializeWebChannel();
    }

public:
    QJsonDocument jsValueToJson(const QJSValue& value);

private:
    void loadEmptyPage();
    void initializeWebChannel();

public:
    JSWebEngineExecutor* q_ptr = nullptr;
    QWebEnginePage m_webPage;
    QWebChannel* m_Channel = nullptr;
    WebClass* m_WebObj = nullptr;
};

JSWebEngineExecutor::JSWebEngineExecutor(QObject* parent)
    : IJsExecutor(parent), d_ptr(new JSWebEngineExecutorPrivate(this)) {
    Q_D(JSWebEngineExecutor);
    qRegisterMetaType<IJsExecutor::ExecuteCallback>();
}

JSWebEngineExecutor::~JSWebEngineExecutor() {}

void JSWebEngineExecutor::injectJson(const QString& name, const QVariant& data) {
    Q_D(JSWebEngineExecutor);

    QJsonDocument doc = QJsonDocument::fromVariant(data);
    QString code = name.toLocal8Bit() + "=" + doc.toJson(QJsonDocument::Compact) + ";";

    d->m_webPage.runJavaScript(code, WORLD_ID, [](const QVariant&) {});
}

void JSWebEngineExecutor::executeApplicationScript(const QByteArray& script, const QUrl& /*sourceUrl*/) {
    Q_D(JSWebEngineExecutor);

    d->m_webPage.runJavaScript(script, WORLD_ID, [=](const QVariant& v) {
        if (v.isValid()) {
        }

        Q_EMIT applicationScriptDone();
    });
}

void JSWebEngineExecutor::executeJSCall(const QString& method,
                                        const QVariantList& args,
                                        const IJsExecutor::ExecuteCallback& callback) {
    Q_D(JSWebEngineExecutor);

    QByteArrayList stringifiedArgs;
    for (const QVariant& arg : args) {
        if (arg.type() == QVariant::List || arg.type() == QVariant::Map) {
            QJsonDocument doc = QJsonDocument::fromVariant(arg);
            QByteArray text = doc.toJson(QJsonDocument::Compact);
            stringifiedArgs += text;
        } else {
            stringifiedArgs += '"' + arg.toString().toLocal8Bit() + '"';
        }
    }
    QByteArray code = QByteArray("__fbBatchedBridge.") + method.toLocal8Bit() + "(" + stringifiedArgs.join(',') + ");";

    d->m_webPage.runJavaScript(code, WORLD_ID, [=](const QVariant& v) {
        QJsonDocument doc;
        if (v != "undefined") {
            doc = QJsonDocument::fromVariant(v);
        }
        callback(doc);
    });
}

void JSWebEngineExecutor::init() {}

void JSWebEngineExecutor::resetConnection() {}

QJsonDocument JSWebEngineExecutorPrivate::jsValueToJson(const QJSValue& value) {

    QVariant variant = value.toVariant();
    if (variant.isValid()) {
        QJsonDocument doc = QJsonDocument::fromVariant(variant);
        return doc;
    }

    return QJsonDocument();
}

void JSWebEngineExecutorPrivate::loadEmptyPage() {
    // without following react doesn't show js exceptions happened in code correctly.
    QEventLoop* eventLoop = new QEventLoop();
    connect(&m_webPage, &QWebEnginePage::loadFinished, this, [=](bool finished) { eventLoop->exit(); });
    m_webPage.setHtml(QString(""));
    eventLoop->exec();
}

void JSWebEngineExecutorPrivate::initializeWebChannel() {
    m_WebObj = new WebClass();
    m_Channel = new QWebChannel(this);
    m_Channel->registerObject("webobj", m_WebObj);
    m_webPage.setWebChannel(m_Channel);

    QFile apiFile(":/qtwebchannel/qwebchannel.js"); // load the API from the resources
    if (!apiFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Couldn't load Qt's QWebChannel API!";
        return;
    }
    QString apiScript = QString::fromLatin1(apiFile.readAll());
    apiFile.close();

    m_webPage.runJavaScript(apiScript, WORLD_ID, [=](const QVariant&) {
        m_webPage.runJavaScript(INIT_CHANNEL_FROM_JS, WORLD_ID, [=](const QVariant&) {});
    });
}
