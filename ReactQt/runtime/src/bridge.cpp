
/**
 * Copyright (C) 2016, Canonical Ltd.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * Author: Justin McPherson <justin.mcpherson@canonical.com>
 *
 */

#include "bridge.h"
#include "alert.h"
#include "appstate.h"
#include "asynclocalstorage.h"
#include "blobprovider.h"
#include "clipboard.h"
#include "communication/javascriptcoreexecutor.h"
#include "communication/jswebengineexecutor.h"
#include "communication/nodejsexecutor.h"
#include "communication/serverconnection.h"
#include "communication/websocketexecutor.h"
#include "componentmanagers/activityindicatormanager.h"
#include "componentmanagers/buttonmanager.h"
#include "componentmanagers/imageloader.h"
#include "componentmanagers/imagemanager.h"
#include "componentmanagers/modalmanager.h"
#include "componentmanagers/navigatormanager.h"
#include "componentmanagers/pickermanager.h"
#include "componentmanagers/rawtextmanager.h"
#include "componentmanagers/scrollviewmanager.h"
#include "componentmanagers/slidermanager.h"
#include "componentmanagers/switchmanager.h"
#include "componentmanagers/textinputmanager.h"
#include "componentmanagers/textmanager.h"
#include "componentmanagers/viewmanager.h"
#include "componentmanagers/webviewmanager.h"
#include "deviceinfo.h"
#include "eventdispatcher.h"
#include "exceptionsmanager.h"
#include "linkingmanager.h"
#include "moduledata.h"
#include "moduleinterface.h"
#include "moduleloader.h"
#include "modulemethod.h"
#include "netinfo.h"
#include "networking.h"
#include "platform.h"
#include "redbox.h"
#include "sourcecode.h"
#include "testmodule.h"
#include "timing.h"
#include "uimanager.h"
#include "utilities.h"
#include "websocketmodule.h"

#ifdef JAVASCRIPTCORE_ENABLED
#include "jscutilities.h"
#endif

#include <QDir>
#include <QJsonDocument>
#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkDiskCache>
#include <QPluginLoader>
#include <QQuickItem>
#include <QStandardPaths>
#include <QThread>
#include <QTimer>

class BridgePrivate {
public:
    bool ready = false;
    bool jsAppStarted = false;
    QString serverConnectionType = "NetExecutor";
    QString jsExecutor = "NodeJsExecutor";
    IJsExecutor* executor = nullptr;
    QQmlEngine* qmlEngine = nullptr;
    QQuickItem* visualParent = nullptr;
    Redbox* redbox = nullptr;
    QNetworkAccessManager* nam = nullptr;
    UIManager* uiManager = nullptr;
    ImageLoader* imageLoader = nullptr;
    SourceCode* sourceCode = nullptr;
    EventDispatcher* eventDispatcher = nullptr;
    TestModule* reactTestModule = nullptr;
    QUrl bundleUrl;
    QString pluginsPath = "./plugins";
    QMap<int, ModuleData*> modules;
    bool remoteJSDebugging = false;
    bool hotReload = false;
    QVariantList externalModules;
    QThread* executorThread = nullptr;

    bool useJSC = false;

    QObjectList internalModules() {
        return QObjectList{new Timing,
                           new AppState,
                           new AsyncLocalStorage,
                           new Networking,
                           new NetInfo,
                           new Clipboard,
                           new LinkingManager,
                           new Alert,
                           new DeviceInfo,
                           new Platform,
                           new BlobProvider,
                           new ViewManager,
                           new RawTextManager,
                           new TextManager,
                           new ImageManager,
                           new ExceptionsManager,
                           new ScrollViewManager,
                           new NavigatorManager,
                           new ActivityIndicatorManager,
                           new TextInputManager,
                           new ButtonManager,
                           new SwitchManager,
                           new SliderManager,
                           new ModalManager,
                           new PickerManager,
                           new WebSocketModule,
                           new WebViewManager};
    }
};

Bridge::Bridge(QObject* parent) : QObject(parent), d_ptr(new BridgePrivate) {
    Q_D(Bridge);

    d->eventDispatcher = new EventDispatcher(this);
}

Bridge::~Bridge() {
    resetExecutor();
    if (d_ptr->executorThread) {
        d_ptr->executorThread->deleteLater();
        d_ptr->executorThread = nullptr;
    }
}

void* Bridge::getJavaScriptContext() {
    Q_D(Bridge);
#ifdef JAVASCRIPTCORE_ENABLED
    JavaScriptCoreExecutor* javaScriptExecutor = qobject_cast<JavaScriptCoreExecutor*>(d->executor);
    if (javaScriptExecutor) {
        return javaScriptExecutor->getJavaScriptContext();
    } else {
        return nullptr;
    }
#else
    return nullptr;
#endif
}

void Bridge::executeOnJavaScriptThread(std::function<void()> func) {
    Q_D(Bridge);
#ifdef JAVASCRIPTCORE_ENABLED
    JavaScriptCoreExecutor* javaScriptExecutor = qobject_cast<JavaScriptCoreExecutor*>(d->executor);
    if (javaScriptExecutor) {
        javaScriptExecutor->executeOnJavaScriptThread(func);
    }
#endif
}

void Bridge::setupExecutor() {
    Q_D(Bridge);

    resetExecutor();
#ifdef RCT_DEV
    if (d->remoteJSDebugging) {
        d->executor = new WebSocketExecutor(QUrl("ws://localhost:8081/debugger-proxy?role=client"), this);
    }
#endif // RCT_DEV

#ifdef JAVASCRIPTCORE_ENABLED
    if (!d->executor) {
        d->useJSC = true;
        JavaScriptCoreExecutor* javaScriptExecutor = new JavaScriptCoreExecutor(this);
        javaScriptExecutor->setBridge(this);
        d->executor = javaScriptExecutor;
    }
#endif // JAVASCRIPTCORE_ENABLED

    if (!d->executor) {

        if (d->jsExecutor == "JSWebEngineExecutor") {
            d->executor = new JSWebEngineExecutor();
        } else if (d->jsExecutor == "NodeJsExecutor") {
            if (!d->executorThread) {
                d->executorThread = new QThread();
            }
            d->executorThread->start();
            ServerConnection* conn =
                qobject_cast<ServerConnection*>(utilities::createQObjectInstance(d->serverConnectionType));

            if (conn == nullptr) {
                qWarning() << __PRETTY_FUNCTION__ << "Could not construct connection: " << d->serverConnectionType
                           << "constructing default (LocalServerConnection)";
                conn = new LocalServerConnection();
            }

            d->executor = new NodeJsExecutor(conn);
            conn->moveToThread(d->executorThread);
            d->executor->moveToThread(d->executorThread);
        }
    }

    connect(d->executor, SIGNAL(applicationScriptDone()), SLOT(applicationScriptDone()));
    QMetaObject::invokeMethod(d_func()->executor, "init", Qt::AutoConnection);
}

void Bridge::resetExecutor() {
    Q_D(Bridge);

    if (d->executor) {
        QMetaObject::invokeMethod(d_func()->executor, "resetConnection", Qt::AutoConnection);
        d->executor->deleteLater();
        d->executor = nullptr;
        d->useJSC = false;
    }
}

void Bridge::init() {
    Q_D(Bridge);

    setupExecutor();
    if (!d->useJSC) {
        initModules();
        injectModules();
        loadSource();
    }
}

void Bridge::reload() {
    Q_D(Bridge);

    setReady(false);
    setJsAppStarted(false);

    setupExecutor();

    d->uiManager->reset();
    for (auto& md : d->modules) {
        md->deleteLater();
    }
    d->modules.clear();
    initModules();
    injectModules();
    loadSource();
}

void Bridge::loadBundle(const QUrl& bundleUrl) {
    setBundleUrl(bundleUrl);
    reload();
}

void Bridge::reset() {
    setReady(false);
    setJsAppStarted(false);
    resetExecutor();
}

void Bridge::enqueueJSCall(const QString& module, const QString& method, const QVariantList& args) {
    if (!d_func()->executor)
        return;
    QVariantList list = QVariantList{module, method, args};
    QMetaObject::invokeMethod(
        d_func()->executor,
        "executeJSCall",
        Qt::AutoConnection,
        Q_ARG(const QString&, "callFunctionReturnFlushedQueue"),
        Q_ARG(const QVariantList&, list),
        Q_ARG(const IJsExecutor::ExecuteCallback&, [=](const QJsonDocument& doc) { processResult(doc); }));
}

void Bridge::invokePromiseCallback(double callbackCode, const QVariantList& args) {
    if (!d_func()->executor)
        return;
    QVariantList list = QVariantList{callbackCode, args};
    QMetaObject::invokeMethod(
        d_func()->executor,
        "executeJSCall",
        Qt::AutoConnection,
        Q_ARG(const QString&, "invokeCallbackAndReturnFlushedQueue"),
        Q_ARG(const QVariantList&, list),
        Q_ARG(const IJsExecutor::ExecuteCallback&, [=](const QJsonDocument& doc) { processResult(doc); }));
}

void Bridge::invokeAndProcess(const QString& method, const QVariantList& args) {
    if (!d_func()->executor)
        return;
    QMetaObject::invokeMethod(
        d_func()->executor,
        "executeJSCall",
        Qt::AutoConnection,
        Q_ARG(const QString&, method),
        Q_ARG(const QVariantList&, args),
        Q_ARG(const IJsExecutor::ExecuteCallback&, [=](const QJsonDocument& doc) { processResult(doc); }));
}

void Bridge::executeSourceCode(const QByteArray& sourceCode) {
    Q_UNUSED(sourceCode);
}

void Bridge::enqueueRunAppCall(const QVariantList& args) {
    if (!d_func()->executor)
        return;

    QVariantList list = QVariantList{"AppRegistry", "runApplication", args};
    QMetaObject::invokeMethod(d_func()->executor,
                              "executeJSCall",
                              Qt::AutoConnection,
                              Q_ARG(QString, "callFunctionReturnFlushedQueue"),
                              Q_ARG(QVariantList, list),
                              Q_ARG(IJsExecutor::ExecuteCallback, [=](const QJsonDocument& doc) {
                                  processResult(doc);
                                  setJsAppStarted(true);
                              }));
}

bool Bridge::ready() const {
    return d_func()->ready;
}

void Bridge::setReady(bool ready) {
    Q_D(Bridge);
    if (d->ready == ready)
        return;

    d->ready = ready;
    emit readyChanged();
}

bool Bridge::jsAppStarted() const {
    return d_func()->jsAppStarted;
}

void Bridge::setJsAppStarted(bool started) {
    Q_D(Bridge);
    if (d->jsAppStarted == started)
        return;

    d->jsAppStarted = started;
    emit jsAppStartedChanged();
}

QQuickItem* Bridge::visualParent() const {
    return d_func()->visualParent;
}

void Bridge::setVisualParent(QQuickItem* item) {
    Q_D(Bridge);

    if (d->visualParent == item)
        return;
    d->visualParent = item;
}

QQuickItem* Bridge::topmostVisualParent() const {
    return visualParent()->parentItem();
}

QQmlEngine* Bridge::qmlEngine() const {
    return d_func()->qmlEngine;
}

void Bridge::setQmlEngine(QQmlEngine* qmlEngine) {
    Q_D(Bridge);
    if (d->qmlEngine == qmlEngine)
        return;
    d->qmlEngine = qmlEngine;
}

QNetworkAccessManager* Bridge::networkAccessManager() const {
    return d_func()->nam;
}

void Bridge::setNetworkAccessManager(QNetworkAccessManager* nam) {
    Q_D(Bridge);
    if (d->nam == nam)
        return;
    d->nam = nam;

    if (d->nam->cache() == nullptr) {
        // TODO: cache size
        auto cache = new QNetworkDiskCache(d->nam);
        cache->setCacheDirectory(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
        d->nam->setCache(cache);
    }
}

QUrl Bridge::bundleUrl() const {
    return d_func()->bundleUrl;
}

void Bridge::setBundleUrl(const QUrl& bundleUrl) {
    Q_D(Bridge);
    if (d->bundleUrl == bundleUrl)
        return;
    d->bundleUrl = bundleUrl;
}

QString Bridge::pluginsPath() const {
    return d_func()->pluginsPath;
}

void Bridge::setPluginsPath(const QString& pluginsPath) {
    Q_D(Bridge);
    if (d->pluginsPath == pluginsPath)
        return;
    d->pluginsPath = pluginsPath;
}

QString Bridge::jsExecutor() const {
    return d_func()->jsExecutor;
}

void Bridge::setJsExecutor(const QString& jsExecutor) {
    Q_D(Bridge);
    if (d->jsExecutor == jsExecutor)
        return;
    d->jsExecutor = jsExecutor;
}

QString Bridge::serverConnectionType() const {
    return d_func()->serverConnectionType;
}

void Bridge::setServerConnectionType(const QString& executorName) {
    Q_D(Bridge);
    if (d->serverConnectionType == executorName)
        return;
    d->serverConnectionType = executorName;
}

const QVariantList& Bridge::externalModules() const {
    return d_func()->externalModules;
}

void Bridge::setExternalModules(const QVariantList& externalModules) {
    Q_D(Bridge);
    d->externalModules = externalModules;
}

EventDispatcher* Bridge::eventDispatcher() const {
    return d_func()->eventDispatcher;
}

QList<ModuleData*> Bridge::modules() const {
    return d_func()->modules.values();
}

UIManager* Bridge::uiManager() const {
    return d_func()->uiManager;
}

TestModule* Bridge::testModule() const {
    return d_func()->reactTestModule;
}

ImageLoader* Bridge::imageLoader() const {
    return d_func()->imageLoader;
}

Redbox* Bridge::redbox() {
    Q_D(Bridge);
    if (d->redbox == nullptr)
        d->redbox = new Redbox(this);
    return d->redbox;
}

void Bridge::setRemoteJSDebugging(bool value) {
    d_func()->remoteJSDebugging = value;
}

void Bridge::setHotReload(bool value) {
    d_func()->hotReload = value;
}

void Bridge::sourcesFinished() {
    Q_D(Bridge);
    QTimer::singleShot(0, [=] {
        QMetaObject::invokeMethod(d->executor,
                                  "executeApplicationScript",
                                  Qt::AutoConnection,
                                  Q_ARG(QByteArray, d->sourceCode->sourceCode()),
                                  Q_ARG(QUrl, d->bundleUrl));
        if (d_func()->hotReload) {
            QVariantList args = QVariantList{"HMRClient",
                                             "enable",
                                             QVariantList{"desktop",
                                                          d->sourceCode->scriptUrl().path().mid(1),
                                                          d->sourceCode->scriptUrl().host(),
                                                          d->sourceCode->scriptUrl().port(0)}};
            QMetaObject::invokeMethod(d_func()->executor,
                                      "executeJSCall",
                                      Qt::AutoConnection,
                                      Q_ARG(const QString&, "callFunctionReturnFlushedQueue"),
                                      Q_ARG(const QVariantList&, args),
                                      Q_ARG(const IJsExecutor::ExecuteCallback&, [=](const QJsonDocument& doc) {
                                          qDebug() << "Enabling HMRClient response";
                                          processResult(doc);
                                      }));
        }
    });
}

void Bridge::sourcesLoadFailed() {
    Q_D(Bridge);
    redbox()->showErrorMessage("Failed to load source code");
}

void Bridge::loadSource() {
    Q_D(Bridge);
    if (d->nam == nullptr) {
        qCritical() << "No QNetworkAccessManager for loading sources";
        return;
    }
    d->sourceCode->loadSource(d->nam);
}

void Bridge::initModules() {
    Q_D(Bridge);

    QObjectList modules;
    modules << d->internalModules();

    // Special cases // TODO:
    d->sourceCode = new SourceCode;
    modules << d->sourceCode;
    d->imageLoader = new ImageLoader;
    modules << d->imageLoader;
    d->reactTestModule = new TestModule;
    modules << d->reactTestModule;

    // XXX:
    d->sourceCode->setScriptUrl(d->bundleUrl);
    connect(d->sourceCode, SIGNAL(sourceCodeChanged()), SLOT(sourcesFinished()));
    connect(d->sourceCode, SIGNAL(loadFailed()), SLOT(sourcesLoadFailed()));

    loadExternalModules(&modules);

    // XXX:
    for (QObject* o : modules) {
        addModuleData(o);
    }

    // Setup of UIManager should be in the end,
    // since it exposes all view managers data to JS as constants of itself
    d->uiManager = new UIManager;
    addModuleData(d->uiManager);
}

void Bridge::addModuleData(QObject* module) {
    ModuleInterface* moduleInterface = qobject_cast<ModuleInterface*>(module);
    if (!moduleInterface) {
        qWarning() << "A module loader exported an invalid module";
        return;
    }

    Q_D(Bridge);

    moduleInterface->setBridge(this);
    ModuleData* moduleData = new ModuleData(module, d->modules.size());
    d->modules.insert(moduleData->id(), moduleData);
}

void Bridge::loadExternalModules(QObjectList* modules) {
    Q_D(Bridge);

    if (modules == nullptr)
        return;

    foreach (QVariant moduleTypeName, d->externalModules) {
        QObject* instance = utilities::createQObjectInstance(moduleTypeName.toString());
        if (!instance) {
            qDebug() << "Can't create QObject instance for external module type name " << moduleTypeName.toString();
            continue;
        }
        ModuleInterface* externalModule = dynamic_cast<ModuleInterface*>(instance);
        if (externalModule) {
            externalModule->setBridge(this);
            modules->append(instance);
        } else {
            qDebug() << "External module " << moduleTypeName.toString() << " must inherit ModuleInterface";
        }
    }
}

void Bridge::injectModules() {
    Q_D(Bridge);

    QVariantList moduleConfig;

    for (int i = 0; i < d->modules.size(); ++i) {
        Q_ASSERT(d->modules.contains(i));
        auto& md = d->modules[i];
        moduleConfig.push_back(md->info());
    }

    QVariant remoteConfig = QVariantMap{{"remoteModuleConfig", moduleConfig}};
    QMetaObject::invokeMethod(d_func()->executor,
                              "injectJson",
                              Qt::AutoConnection,
                              Q_ARG(const QString&, "__fbBatchedBridgeConfig"),
                              Q_ARG(const QVariant&, remoteConfig));
}

void Bridge::processResult(const QJsonDocument& doc) {
    Q_D(Bridge);

    if (doc.isNull())
        return;

    if (!doc.isArray()) {
        qCritical() << __PRETTY_FUNCTION__ << "Returned document from executor in unexpected form";
        return;
    }

    QVariantList requests = doc.toVariant().toList();

    QVariantList moduleIDs = requests[FieldRequestModuleIDs].toList();
    QVariantList methodIDs = requests[FieldMethodIDs].toList();
    QVariantList paramArrays = requests[FieldParams].toList();

    // XXX: this should all really be wrapped up in a Module class
    // including invocations etc
    for (int i = 0; i < moduleIDs.size(); ++i) {
        QMetaObject::invokeMethod(this,
                                  "invokeModuleMethod",
                                  Qt::AutoConnection,
                                  Q_ARG(int, moduleIDs[i].toInt()),
                                  Q_ARG(int, methodIDs[i].toInt()),
                                  Q_ARG(QList<QVariant>, paramArrays[i].toList()));
    }
}

void Bridge::invokeModuleMethod(int moduleId, int methodId, QList<QVariant> args) {
    Q_D(Bridge);

    ModuleData* moduleData = d->modules[moduleId];
    if (moduleData == nullptr) {
        qCritical() << __PRETTY_FUNCTION__ << "Could not find referenced module";
        return;
    }

    ModuleMethod* method = moduleData->method(methodId);
    if (method == nullptr) {
        qCritical() << __PRETTY_FUNCTION__ << "Request for unsupported method";
        return;
    }

    // readable log of methods invoked via bridge
    // qDebug() << "INVOKE: " << moduleData->name() << "::" << method->name() << "( " << args << " )";

    method->invoke(args);
}

void Bridge::applicationScriptDone() {
    QTimer::singleShot(0, [this]() {
        QMetaObject::invokeMethod(d_func()->executor,
                                  "executeJSCall",
                                  Qt::AutoConnection,
                                  Q_ARG(const QString&, "flushedQueue"),
                                  Q_ARG(const QVariantList&, QVariantList()),
                                  Q_ARG(const IJsExecutor::ExecuteCallback&, [=](const QJsonDocument& doc) {
                                      processResult(doc);
                                      setReady(true);
                                  }));
    });
}

void Bridge::partialBatchDidFlush() {}
void Bridge::batchDidComplete() {}

#include "bridge.moc"
