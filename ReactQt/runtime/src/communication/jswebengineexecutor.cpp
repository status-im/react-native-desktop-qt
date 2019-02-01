
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
#include <QWebEnginePage>
#include <QWebEngineSettings>

class JSWebEngineExecutorPrivate : public QObject {
public:
    JSWebEngineExecutorPrivate(JSWebEngineExecutor* e) : QObject(e), q_ptr(e) {}

public:
    QJsonDocument jsValueToJson(const QJSValue& value);

public:
    JSWebEngineExecutor* q_ptr = nullptr;
    QWebEnginePage m_webPage;
};

JSWebEngineExecutor::JSWebEngineExecutor(QObject* parent)
    : IJsExecutor(parent), d_ptr(new JSWebEngineExecutorPrivate(this)) {
    Q_D(JSWebEngineExecutor);
    qRegisterMetaType<IJsExecutor::ExecuteCallback>();

    // without following react doesn't show js exceptions happened in code correctly.
    QEventLoop* eventLoop = new QEventLoop();
    connect(&d->m_webPage, &QWebEnginePage::loadFinished, this, [=](bool finished) { eventLoop->exit(); });
    d->m_webPage.setHtml(QString(""));
    eventLoop->exec();
}

JSWebEngineExecutor::~JSWebEngineExecutor() {}

void JSWebEngineExecutor::injectJson(const QString& name, const QVariant& data) {
    Q_D(JSWebEngineExecutor);

    QJsonDocument doc = QJsonDocument::fromVariant(data);
    QString code = name.toLocal8Bit() + "=" + doc.toJson(QJsonDocument::Compact) + ";";

    d->m_webPage.runJavaScript(code, 0, [](const QVariant& v) {});
}

void JSWebEngineExecutor::executeApplicationScript(const QByteArray& script, const QUrl& /*sourceUrl*/) {
    Q_D(JSWebEngineExecutor);

    d->m_webPage.runJavaScript(script, 0, [=](const QVariant& v) {
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

    d->m_webPage.runJavaScript(code, 0, [=](const QVariant& v) {
        QJsonDocument doc;
        if (v != "undefined") {
            //            qDebug() << "========== executeJSCall result: ==========";
            //            qDebug() << v;

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
