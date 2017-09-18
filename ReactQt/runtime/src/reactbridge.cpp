
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

#include "reactbridge.h"
#include "reactmoduledata.h"
#include "reactmoduleinterface.h"
#include "reactmoduleloader.h"
#include "reactmodulemethod.h"
#include "reactnetexecutor.h"
#include "reactsourcecode.h"

#include "reactactivityindicatormanager.h"
#include "reactappstate.h"
#include "reactasynclocalstorage.h"
#include "reactblobprovider.h"
#include "reactbuttonmanager.h"
#include "reactdeviceinfo.h"
#include "reacteventdispatcher.h"
#include "reactexceptionsmanager.h"
#include "reactimageloader.h"
#include "reactimagemanager.h"
#include "reactnavigatormanager.h"
#include "reactnetinfo.h"
#include "reactnetworking.h"
#include "reactrawtextmanager.h"
#include "reactredboxitem.h"
#include "reactscrollviewmanager.h"
#include "reacttestmodule.h"
#include "reacttextmanager.h"
#include "reacttiming.h"
#include "reactuimanager.h"
#include "reactviewmanager.h"

class ReactBridgePrivate {
public:
    bool ready = false;
    bool jsAppStarted = false;
    QString executorName = "ReactNetExecutor";
    ReactExecutor* executor = nullptr;
    QQmlEngine* qmlEngine = nullptr;
    QQuickItem* visualParent = nullptr;
    ReactRedboxItem* redbox = nullptr;
    QNetworkAccessManager* nam = nullptr;
    ReactUIManager* uiManager = nullptr;
    ReactImageLoader* imageLoader = nullptr;
    ReactSourceCode* sourceCode = nullptr;
    ReactEventDispatcher* eventDispatcher = nullptr;
    ReactTestModule* reactTestModule = nullptr;
    QUrl bundleUrl;
    QString pluginsPath = "./plugins";
    QMap<int, ReactModuleData*> modules;

    QObjectList internalModules() {
        return QObjectList{
            new ReactTiming,
            new ReactAppState,
            new ReactAsyncLocalStorage,
            new ReactNetworking,
            new ReactNetInfo,
            new ReactDeviceInfo,
            new ReactBlobProvider,
            new ReactViewManager,
            new ReactRawTextManager,
            new ReactTextManager,
            new ReactImageManager,
            new ReactExceptionsManager,
            new ReactScrollViewManager,
            new ReactNavigatorManager,
            new ReactActivityIndicatorManager,
            new ReactButtonManager,
        };
    }
};

ReactBridge::ReactBridge(QObject* parent) : QObject(parent), d_ptr(new ReactBridgePrivate) {
    Q_D(ReactBridge);

    d->eventDispatcher = new ReactEventDispatcher(this);
}

ReactBridge::~ReactBridge() {}

void ReactBridge::setupExecutor() {
    Q_D(ReactBridge);

    // Find executor
    const int executorType = QMetaType::type((d->executorName + "*").toLocal8Bit());
    if (executorType != QMetaType::UnknownType) {
        d->executor = qobject_cast<ReactExecutor*>(
            QMetaType::metaObjectForType(executorType)->newInstance(Q_ARG(QObject*, this)));
    }

    if (d->executor == nullptr) {
        qWarning() << __PRETTY_FUNCTION__ << "Could not construct executor named" << d->executorName
                   << "constructing default (ReactNetExecutor)";
        d->executor = new ReactNetExecutor(this); // TODO: config/property
    }

    connect(d->executor, SIGNAL(applicationScriptDone()), SLOT(applicationScriptDone()));
    d->executor->init();
}

void ReactBridge::init() {
    Q_D(ReactBridge);

    setupExecutor();
    initModules();
    injectModules();
    loadSource();
}

void ReactBridge::reload() {
    Q_D(ReactBridge);

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

void ReactBridge::loadBundle(const QUrl& bundleUrl) {
    setBundleUrl(bundleUrl);
    reload();
}

void ReactBridge::enqueueJSCall(const QString& module, const QString& method, const QVariantList& args) {
    d_func()->executor->executeJSCall("callFunctionReturnFlushedQueue",
                                      QVariantList{module, method, args},
                                      [=](const QJsonDocument& doc) { processResult(doc); });
}

void ReactBridge::invokePromiseCallback(double callbackCode, const QVariantList& args) {
    d_func()->executor->executeJSCall("invokeCallbackAndReturnFlushedQueue",
                                      QVariantList{callbackCode, args},
                                      [=](const QJsonDocument& doc) { processResult(doc); });
}

void ReactBridge::invokeAndProcess(const QString& method, const QVariantList& args) {
    d_func()->executor->executeJSCall(method, args, [=](const QJsonDocument& doc) { processResult(doc); });
}

void ReactBridge::executeSourceCode(const QByteArray& sourceCode) {
    Q_UNUSED(sourceCode);
}

void ReactBridge::enqueueRunAppCall(const QVariantList& args) {
    d_func()->executor->executeJSCall("callFunctionReturnFlushedQueue",
                                      QVariantList{"AppRegistry", "runApplication", args},
                                      [=](const QJsonDocument& doc) {
                                          processResult(doc);
                                          setJsAppStarted(true);
                                      });
}

bool ReactBridge::ready() const {
    return d_func()->ready;
}

void ReactBridge::setReady(bool ready) {
    Q_D(ReactBridge);
    if (d->ready == ready)
        return;

    d->ready = ready;
    emit readyChanged();
}

bool ReactBridge::jsAppStarted() const {
    return d_func()->jsAppStarted;
}

void ReactBridge::setJsAppStarted(bool started) {
    Q_D(ReactBridge);
    if (d->jsAppStarted == started)
        return;

    d->jsAppStarted = started;
    emit jsAppStartedChanged();
}

QQuickItem* ReactBridge::visualParent() const {
    return d_func()->visualParent;
}

void ReactBridge::setVisualParent(QQuickItem* item) {
    Q_D(ReactBridge);
    if (d->visualParent == item)
        return;
    d->visualParent = item;
}

QQmlEngine* ReactBridge::qmlEngine() const {
    return d_func()->qmlEngine;
}

void ReactBridge::setQmlEngine(QQmlEngine* qmlEngine) {
    Q_D(ReactBridge);
    if (d->qmlEngine == qmlEngine)
        return;
    d->qmlEngine = qmlEngine;
}

QNetworkAccessManager* ReactBridge::networkAccessManager() const {
    return d_func()->nam;
}

void ReactBridge::setNetworkAccessManager(QNetworkAccessManager* nam) {
    Q_D(ReactBridge);
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

QUrl ReactBridge::bundleUrl() const {
    return d_func()->bundleUrl;
}

void ReactBridge::setBundleUrl(const QUrl& bundleUrl) {
    Q_D(ReactBridge);
    if (d->bundleUrl == bundleUrl)
        return;
    d->bundleUrl = bundleUrl;
}

QString ReactBridge::pluginsPath() const {
    return d_func()->pluginsPath;
}

void ReactBridge::setPluginsPath(const QString& pluginsPath) {
    Q_D(ReactBridge);
    if (d->pluginsPath == pluginsPath)
        return;
    d->pluginsPath = pluginsPath;
}

QString ReactBridge::executorName() const {
    return d_func()->executorName;
}

void ReactBridge::setExecutorName(const QString& executorName) {
    Q_D(ReactBridge);
    if (d->executorName == executorName)
        return;
    d->executorName = executorName;
}

ReactEventDispatcher* ReactBridge::eventDispatcher() const {
    return d_func()->eventDispatcher;
}

QList<ReactModuleData*> ReactBridge::modules() const {
    return d_func()->modules.values();
}

ReactUIManager* ReactBridge::uiManager() const {
    return d_func()->uiManager;
}

ReactTestModule* ReactBridge::testModule() const {
    return d_func()->reactTestModule;
}

ReactImageLoader* ReactBridge::imageLoader() const {
    return d_func()->imageLoader;
}

ReactRedboxItem* ReactBridge::redbox() {
    Q_D(ReactBridge);
    if (d->redbox == nullptr)
        d->redbox = new ReactRedboxItem(this);
    return d->redbox;
}

void ReactBridge::sourcesFinished() {
    Q_D(ReactBridge);
    QTimer::singleShot(0, [=] { d->executor->executeApplicationScript(d->sourceCode->sourceCode(), d->bundleUrl); });
}

void ReactBridge::sourcesLoadFailed() {
    Q_D(ReactBridge);
    redbox()->showErrorMessage("Failed to load source code");
}

void ReactBridge::loadSource() {
    Q_D(ReactBridge);
    if (d->nam == nullptr) {
        qCritical() << "No QNetworkAccessManager for loading sources";
        return;
    }
    d->sourceCode->loadSource(d->nam);
}

void ReactBridge::initModules() {
    Q_D(ReactBridge);

    QObjectList modules;
    modules << d->internalModules();

    // Special cases // TODO:
    d->sourceCode = new ReactSourceCode;
    modules << d->sourceCode;
    d->imageLoader = new ReactImageLoader;
    modules << d->imageLoader;
    d->reactTestModule = new ReactTestModule;
    modules << d->reactTestModule;
    d->uiManager = new ReactUIManager; // XXX: this needs to be at end, FIXME:
    modules << d->uiManager;

    // XXX:
    d->sourceCode->setScriptUrl(d->bundleUrl);
    connect(d->sourceCode, SIGNAL(sourceCodeChanged()), SLOT(sourcesFinished()));
    connect(d->sourceCode, SIGNAL(loadFailed()), SLOT(sourcesLoadFailed()));

    // XXX:
    for (QObject* o : modules) {
        ReactModuleInterface* module = qobject_cast<ReactModuleInterface*>(o);
        if (module != nullptr) {
            module->setBridge(this);
            ReactModuleData* moduleData = new ReactModuleData(o, d->modules.size());
            d->modules.insert(moduleData->id(), moduleData);
        } else {
            qWarning() << "A module loader exported an invalid module";
        }
    }
}

void ReactBridge::injectModules() {
    Q_D(ReactBridge);

    QVariantList moduleConfig;

    for (int i = 0; i < d->modules.size(); ++i) {
        //    qDebug() << "Injecting module" << md->name();
        Q_ASSERT(d->modules.contains(i));
        auto& md = d->modules[i];
        moduleConfig.push_back(md->info());
    }

    d->executor->injectJson("__fbBatchedBridgeConfig", QVariantMap{{"remoteModuleConfig", moduleConfig}});
}

void ReactBridge::processResult(const QJsonDocument& doc) {
    Q_D(ReactBridge);

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

    // qDebug() << "moduleIDs" << moduleIDs;
    // qDebug() << "methodIDs" << methodIDs;
    // qDebug() << "paramArrays" << paramArrays;

    // XXX: this should all really be wrapped up in a Module class
    // including invocations etc
    for (int i = 0; i < moduleIDs.size(); ++i) {
        invokeModuleMethod(moduleIDs[i].toInt(), methodIDs[i].toInt(), paramArrays[i].toList());
    }
}

void ReactBridge::invokeModuleMethod(int moduleId, int methodId, QList<QVariant> args) {
    Q_D(ReactBridge);

    ReactModuleData* moduleData = d->modules[moduleId];
    if (moduleData == nullptr) {
        qCritical() << __PRETTY_FUNCTION__ << "Could not find referenced module";
        return;
    }

    ReactModuleMethod* method = moduleData->method(methodId);
    if (method == nullptr) {
        qCritical() << __PRETTY_FUNCTION__ << "Request for unsupported method";
        return;
    }

    // readable log of methods invoked via bridge
    // qDebug() << "INVOKE: " << moduleData->name() << "::" << method->name() << "( " << args << " )";

    method->invoke(args);
}

void ReactBridge::applicationScriptDone() {
    QTimer::singleShot(0, [this]() {
        d_func()->executor->executeJSCall("flushedQueue",
                                          QVariantList{},
                                          [=](const QJsonDocument& doc) {
                                              processResult(doc);
                                              setReady(true);
                                          });
    });
}
