
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

#include <QDir>
#include <QJsonDocument>
#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkDiskCache>
#include <QPluginLoader>
#include <QQuickItem>
#include <QStandardPaths>
#include <QTimer>

#include "bridge.h"
#include "moduledata.h"
#include "moduleinterface.h"
#include "moduleloader.h"
#include "modulemethod.h"
#include "netexecutor.h"
#include "sourcecode.h"

#include "appstate.h"
#include "asynclocalstorage.h"
#include "blobprovider.h"
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
#include "componentmanagers/textinputmanager.h"
#include "componentmanagers/textmanager.h"
#include "componentmanagers/viewmanager.h"
#include "deviceinfo.h"
#include "eventdispatcher.h"
#include "exceptionsmanager.h"
#include "netinfo.h"
#include "networking.h"

#include "redboxitem.h"
#include "testmodule.h"
#include "timing.h"
#include "uimanager.h"

class BridgePrivate {
public:
    bool ready = false;
    bool jsAppStarted = false;
    QString executorName = "NetExecutor";
    Executor* executor = nullptr;
    QQmlEngine* qmlEngine = nullptr;
    QQuickItem* visualParent = nullptr;
    RedboxItem* redbox = nullptr;
    QNetworkAccessManager* nam = nullptr;
    UIManager* uiManager = nullptr;
    ImageLoader* imageLoader = nullptr;
    SourceCode* sourceCode = nullptr;
    EventDispatcher* eventDispatcher = nullptr;
    TestModule* reactTestModule = nullptr;
    QUrl bundleUrl;
    QString pluginsPath = "./plugins";
    QMap<int, ModuleData*> modules;

    QObjectList internalModules() {
        return QObjectList{new Timing,
                           new AppState,
                           new AsyncLocalStorage,
                           new Networking,
                           new NetInfo,
                           new DeviceInfo,
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
                           new SliderManager,
                           new ModalManager,
                           new PickerManager};
    }
};

Bridge::Bridge(QObject* parent) : QObject(parent), d_ptr(new BridgePrivate) {
    Q_D(Bridge);

    d->eventDispatcher = new EventDispatcher(this);
}

Bridge::~Bridge() {}

void Bridge::setupExecutor() {
    Q_D(Bridge);

    // Find executor
    const int executorType = QMetaType::type((d->executorName + "*").toLocal8Bit());
    if (executorType != QMetaType::UnknownType) {
        d->executor =
            qobject_cast<Executor*>(QMetaType::metaObjectForType(executorType)->newInstance(Q_ARG(QObject*, this)));
    }

    if (d->executor == nullptr) {
        qWarning() << __PRETTY_FUNCTION__ << "Could not construct executor named" << d->executorName
                   << "constructing default (ReactNetExecutor)";
        d->executor = new NetExecutor(this); // TODO: config/property
    }

    connect(d->executor, SIGNAL(applicationScriptDone()), SLOT(applicationScriptDone()));
    d->executor->init();
}

void Bridge::init() {
    Q_D(Bridge);

    setupExecutor();
    initModules();
    injectModules();
    loadSource();
}

void Bridge::reload() {
    Q_D(Bridge);

    setReady(false);
    setJsAppStarted(false);

    d->executor->deleteLater();
    setupExecutor();

    // d->uiManager->reset();
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

void Bridge::enqueueJSCall(const QString& module, const QString& method, const QVariantList& args) {
    d_func()->executor->executeJSCall("callFunctionReturnFlushedQueue",
                                      QVariantList{module, method, args},
                                      [=](const QJsonDocument& doc) { processResult(doc); });
}

void Bridge::invokePromiseCallback(double callbackCode, const QVariantList& args) {
    d_func()->executor->executeJSCall("invokeCallbackAndReturnFlushedQueue",
                                      QVariantList{callbackCode, args},
                                      [=](const QJsonDocument& doc) { processResult(doc); });
}

void Bridge::invokeAndProcess(const QString& method, const QVariantList& args) {
    d_func()->executor->executeJSCall(method, args, [=](const QJsonDocument& doc) { processResult(doc); });
}

void Bridge::executeSourceCode(const QByteArray& sourceCode) {
    Q_UNUSED(sourceCode);
}

void Bridge::enqueueRunAppCall(const QVariantList& args) {
    d_func()->executor->executeJSCall("callFunctionReturnFlushedQueue",
                                      QVariantList{"AppRegistry", "runApplication", args},
                                      [=](const QJsonDocument& doc) {
                                          processResult(doc);
                                          setJsAppStarted(true);
                                      });
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

QString Bridge::executorName() const {
    return d_func()->executorName;
}

void Bridge::setExecutorName(const QString& executorName) {
    Q_D(Bridge);
    if (d->executorName == executorName)
        return;
    d->executorName = executorName;
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

RedboxItem* Bridge::redbox() {
    Q_D(Bridge);
    if (d->redbox == nullptr)
        d->redbox = new RedboxItem(this);
    return d->redbox;
}

void Bridge::sourcesFinished() {
    Q_D(Bridge);
    QTimer::singleShot(0, [=] { d->executor->executeApplicationScript(d->sourceCode->sourceCode(), d->bundleUrl); });
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
    d->uiManager = new UIManager; // XXX: this needs to be at end, FIXME:
    modules << d->uiManager;

    // XXX:
    d->sourceCode->setScriptUrl(d->bundleUrl);
    connect(d->sourceCode, SIGNAL(sourceCodeChanged()), SLOT(sourcesFinished()));
    connect(d->sourceCode, SIGNAL(loadFailed()), SLOT(sourcesLoadFailed()));

    // XXX:
    for (QObject* o : modules) {
        ModuleInterface* module = qobject_cast<ModuleInterface*>(o);
        if (module != nullptr) {
            module->setBridge(this);
            ModuleData* moduleData = new ModuleData(o, d->modules.size());
            d->modules.insert(moduleData->id(), moduleData);
        } else {
            qWarning() << "A module loader exported an invalid module";
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

    d->executor->injectJson("__fbBatchedBridgeConfig", QVariantMap{{"remoteModuleConfig", moduleConfig}});
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
        invokeModuleMethod(moduleIDs[i].toInt(), methodIDs[i].toInt(), paramArrays[i].toList());
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
    //    qDebug() << "INVOKE: " << moduleData->name() << "::" << method->name() << "( " << args << " )";

    method->invoke(args);
}

void Bridge::applicationScriptDone() {
    QTimer::singleShot(0, [this]() {
        d_func()->executor->executeJSCall("flushedQueue",
                                          QVariantList{},
                                          [=](const QJsonDocument& doc) {
                                              processResult(doc);
                                              setReady(true);
                                          });
    });
}
