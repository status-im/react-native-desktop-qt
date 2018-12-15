
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include "webengineqtexecutor.h"
#include "bridge.h"

#include <QJsonDocument>
#include <QSharedPointer>

#include <QStandardPaths>

#include <QWebEnginePage>
#include <QWebEngineView>
#include <QWebEngineProfile>
#include <QWebEnginePage>
#include <QWebEngineScript>
#include <QWebEngineScriptCollection>
#include <QWebEngineSettings>
#include <QDir>
#include <QEventLoop>

#include <QNetworkProxy>

#include <QWebChannel>
#include <QFile>

class WebEngineQtExecutorPrivate : public QObject {
public:
    WebEngineQtExecutorPrivate(WebEngineQtExecutor* e) : QObject(e), q_ptr(e) {}

    WebEngineQtExecutor* q_ptr = nullptr;

    QWebEngineView *webEngine = nullptr;
    QWebEnginePage *myPage = nullptr;
    QWebChannel *myChannel = nullptr;

    QString registerObjectId;
    QObject *registerObject = nullptr;

    void init();
};

void WebEngineQtExecutorPrivate::init() {
    webEngine = new QWebEngineView();

    auto path = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
    qDebug() << "setPersistentStoragePath: " << path;

    auto *profile = new QWebEngineProfile("StatusReactProfile", webEngine);

    QDir storageDir(path);
    storageDir.mkdir("WebEngineTest");

    profile->settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
    profile->setPersistentStoragePath(path + "/WebEngineTest");
    profile->setCachePath(path + "/WebEngineTest");

    QFile webChannelJsFile(":/qtwebchannel/qwebchannel.js");
    if(!webChannelJsFile.open(QIODevice::ReadOnly) )
    {
        //qFatal( QString("Couldn't open qwebchannel.js file: %1").arg(webChannelJsFile.errorString()) );
    }
    else
    {
        QByteArray webChannelJs = webChannelJsFile.readAll();
        QWebEngineScript script;
        script.setSourceCode(webChannelJs);
        script.setName("qwebchannel.js");
        script.setWorldId(QWebEngineScript::MainWorld);
        script.setInjectionPoint(QWebEngineScript::DocumentCreation);
        script.setRunsOnSubFrames(false);
        profile->scripts()->insert(script);
    }

    myPage = new QWebEnginePage(profile, webEngine);

    myChannel = new QWebChannel(this);
    myPage->setWebChannel(myChannel);

    //myChannel->registerObject(registerObjectId, registerObject);

    QEventLoop* eventLoop = new QEventLoop();


    connect(myPage, &QWebEnginePage::loadFinished, this, [=](bool finished) {
        qDebug() << "!!! load finished with result: " << finished;
        eventLoop->exit();
    });

    myPage->setHtml(QString("<html> <head>\n"
                            "         <script type=\"text/javascript\">\n"
                            "           window.onload = function() {\n"
                            "               console.log('123 Test message');\n"
                            "               new QWebChannel(qt.webChannelTransport, function(channel) { \n"
                            "               console.log('!!! new QWebChannel is created');\n"
                            //"               window.QWebEngineRealm = channel.objects.RealmTest;\n"
                            //"               console.log('Custom JSObject= ' + window.QWebEngineRealm);\n"
                            "           }\n);"
                            "          }\n"
                            "         </script>\n"
                            "       </head>\n"
                            "       <body> </body> </html>"
                            ), QUrl("http://wwww.react-native-desktop.com"));
    eventLoop->exec();
}

WebEngineQtExecutor::WebEngineQtExecutor(QObject* parent) : IExecutor(parent), d_ptr(new WebEngineQtExecutorPrivate(this)) {
    qRegisterMetaType<IExecutor::ExecuteCallback>();
    qRegisterMetaType<QNetworkProxy>("QNetworkProxy");
    qRegisterMetaType<QAbstractSocket::SocketError>();
}

void WebEngineQtExecutor::initJSconstraints() {
    d_ptr->init();
}

WebEngineQtExecutor::~WebEngineQtExecutor() {
    resetConnection();
}

void WebEngineQtExecutor::injectJson(const QString& name, const QVariant& data) {
    Q_D(WebEngineQtExecutor);
    QJsonDocument doc = QJsonDocument::fromVariant(data);
    d_ptr->myPage->runJavaScript(name.toLocal8Bit() + "=" + doc.toJson(QJsonDocument::Compact) + ";", 0, [](const QVariant&) {
    });
}

void WebEngineQtExecutor::executeApplicationScript(const QByteArray& script, const QUrl& /*sourceUrl*/) {
    d_ptr->myPage->runJavaScript(script, 0, [=](const QVariant&) {
        Q_EMIT applicationScriptDone();
    });
}

void WebEngineQtExecutor::executeJSCall(const QString& method,
                                        const QVariantList& args,
                                        const IExecutor::ExecuteCallback& callback) {
    QByteArrayList stringifiedArgs;
    for (const QVariant& arg : args) {
        if (arg.type() == QVariant::List || arg.type() == QVariant::Map) {
            QJsonDocument doc = QJsonDocument::fromVariant(arg);
            stringifiedArgs += doc.toJson(QJsonDocument::Compact);
        } else {
            stringifiedArgs += '"' + arg.toString().toLocal8Bit() + '"';
        }
    }

    d_ptr->myPage->runJavaScript(QByteArray("__fbBatchedBridge.") + method.toLocal8Bit() + "(" + stringifiedArgs.join(',') + ");", 0, [=](const QVariant &v) {
        if (v.isValid()) {
            qDebug() << "Result of executeJSCall: " << v;
        }

        QJsonDocument doc;
        if (v != "undefined") {
            doc = QJsonDocument::fromVariant(v);
        }
        callback(doc);
    });
}

void WebEngineQtExecutor::registerJSObject(const QString &id, QObject *object) {
    d_ptr->registerObjectId = id;
    d_ptr->registerObject = object;
}

void WebEngineQtExecutor::init() {
}

void WebEngineQtExecutor::resetConnection() {
}
