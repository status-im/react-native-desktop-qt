
#include <QStandardPaths>
#include <QMap>
#include <QPluginLoader>
#include <QJsonDocument>
#include <QQuickItem>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkDiskCache>

#include "reactbridge.h"
#include "reactsourcecode.h"
#include "reactnetexecutor.h"
#include "reactmoduleloader.h"
#include "reactmoduleinterface.h"
#include "reactmoduledata.h"
#include "reactmodulemethod.h"

#include "reacteventdispatcher.h"
#include "reactnetworking.h"
#include "reacttiming.h"
#include "reactappstate.h"
#include "reactasynclocalstorage.h"
#include "reactviewmanager.h"
#include "reactrawtextmanager.h"
#include "reacttextmanager.h"
#include "reactimagemanager.h"
#include "reactuimanager.h"
#include "reactimageloader.h"

#include "ubuntuscrollviewmanager.h"
#include "ubuntunavigatormanager.h"
#include "ubuntupagemanager.h"
#include "ubuntutextfieldmanager.h"
#include "ubuntudatepickermanager.h"
#include "ubuntucomponentsloader.h"


class ReactBridgePrivate
{
public:
  bool ready = false;
  ReactExecutor* executor = nullptr;
  QQmlEngine* qmlEngine = nullptr;
  QQuickItem* visualParent = nullptr;
  QNetworkAccessManager* nam = nullptr;
  ReactUIManager* uiManager = nullptr;
  ReactImageLoader* imageLoader = nullptr;
  ReactSourceCode* sourceCode = nullptr;
  ReactEventDispatcher* eventDispatcher = nullptr;
  QUrl bundleUrl;
  QMap<int, ReactModuleData*> modules;

  QObjectList internalModules() {
    return QObjectList {
      new ReactTiming,
      new ReactAppState,
      new ReactAsyncLocalStorage,
      new ReactNetworking,
      new ReactViewManager,
      new ReactRawTextManager,
      new ReactTextManager,
      new ReactImageManager,
    };
  }

  QObjectList pluginModules() {
    QObjectList modules;
    UbuntuComponentsLoader loader;
    modules << loader.availableModules();
    // for (QObject* o : QPluginLoader::staticInstances()) {
    //   ReactModuleLoader* ml = qobject_cast<ReactModuleLoader*>(o);
    //   if (o == nullptr)
    //     continue;

    //   modules << ml->availableModules();
    // }
    modules << new UbuntuScrollViewManager; //XXX:
    modules << new UbuntuNavigatorManager;
    modules << new UbuntuPageManager;
    modules << new UbuntuTextFieldManager;
    modules << new UbuntuDatePickerManager;
    return modules;
  }
};


ReactBridge::ReactBridge(QObject* parent)
  : QObject(parent)
  , d_ptr(new ReactBridgePrivate)
{
  Q_D(ReactBridge);

  setupExecutor();

  d->eventDispatcher = new ReactEventDispatcher(this);
}

ReactBridge::~ReactBridge()
{
}

void ReactBridge::setupExecutor() {
  Q_D(ReactBridge);

  d->executor = new ReactNetExecutor(this); // TODO: config/property
  connect(d->executor, SIGNAL(applicationScriptDone()), SLOT(applicationScriptDone()));
}

void ReactBridge::init()
{
  Q_D(ReactBridge);

  d->executor->init();

  initModules();
  injectModules();
  loadSource();
}

void ReactBridge::reload()
{
  Q_D(ReactBridge);

  setReady(false);

  d->executor->deleteLater();
  d->executor = new ReactNetExecutor(this);
  connect(d->executor, SIGNAL(applicationScriptDone()), SLOT(applicationScriptDone()));
  d->executor->init();

  // d->uiManager->reset();
  for (auto& md : d->modules) {
    delete md;
  }
  d->modules.clear();

  initModules();
  injectModules();
  loadSource();
}

void ReactBridge::enqueueJSCall(const QString& module, const QString& method, const QVariantList& args)
{
  d_func()->executor->executeJSCall("callFunctionReturnFlushedQueue",
                                    QVariantList{module, method, args},
                                    [=](const QJsonDocument& doc) {
                                      processResult(doc);
                                    });
}

void ReactBridge::invokeAndProcess(const QString& method, const QVariantList &args)
{
  d_func()->executor->executeJSCall(method, args, [=](const QJsonDocument& doc) { processResult(doc); });
}

void ReactBridge::executeSourceCode(const QByteArray& sourceCode)
{
  Q_UNUSED(sourceCode);
}

bool ReactBridge::ready() const
{
  return d_func()->ready;
}

void ReactBridge::setReady(bool ready)
{
  Q_D(ReactBridge);
  if (d->ready == ready)
    return;

  d->ready = ready;
  emit readyChanged();
}

QQuickItem* ReactBridge::visualParent() const
{
  return d_func()->visualParent;
}

void ReactBridge::setVisualParent(QQuickItem* item)
{
  Q_D(ReactBridge);
  if (d->visualParent == item)
    return;
  d->visualParent = item;
}

QQmlEngine* ReactBridge::qmlEngine() const
{
  return d_func()->qmlEngine;
}

void ReactBridge::setQmlEngine(QQmlEngine* qmlEngine)
{
  Q_D(ReactBridge);
  if (d->qmlEngine == qmlEngine)
    return;
  d->qmlEngine = qmlEngine;
}

QNetworkAccessManager* ReactBridge::networkAccessManager() const
{
  return d_func()->nam;
}

void ReactBridge::setNetworkAccessManager(QNetworkAccessManager* nam)
{
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

QUrl ReactBridge::bundleUrl() const
{
  return d_func()->bundleUrl;
}

void ReactBridge::setBundleUrl(const QUrl& bundleUrl)
{
  Q_D(ReactBridge);
  if (d->bundleUrl == bundleUrl)
    return;
  d->bundleUrl = bundleUrl;
}

ReactEventDispatcher* ReactBridge::eventDispatcher() const
{
  return d_func()->eventDispatcher;
}

QList<ReactModuleData*> ReactBridge::modules() const
{
  return d_func()->modules.values();
}

ReactUIManager* ReactBridge::uiManager() const
{
  return d_func()->uiManager;
}

ReactImageLoader* ReactBridge::imageLoader() const
{
  return d_func()->imageLoader;
}

void ReactBridge::sourcesFinished()
{
  Q_D(ReactBridge);
  // XXX:
  QTimer::singleShot(200, [=]() {
      d->executor->executeApplicationScript(d->sourceCode->sourceCode(), d->bundleUrl);
    });
}

void ReactBridge::loadSource()
{
  Q_D(ReactBridge);
  if (d->nam == nullptr) {
    qCritical() << "No QNetworkAccessManager for loading sources";
    return;
  }
  d->sourceCode->loadSource(d->nam);
}


void ReactBridge::initModules()
{
  Q_D(ReactBridge);

  QObjectList modules;
  modules << d->internalModules();
  modules << d->pluginModules();

  // Special cases // TODO:
  d->sourceCode = new ReactSourceCode;
  modules << d->sourceCode;
  d->imageLoader = new ReactImageLoader;
  modules << d->imageLoader;
  d->uiManager = new ReactUIManager; // XXX: this needs to be at end, FIXME:
  modules << d->uiManager;

  // XXX:
  d->sourceCode->setScriptUrl(d->bundleUrl);
  connect(d->sourceCode, SIGNAL(sourceCodeChanged()), SLOT(sourcesFinished()));

  // XXX:
  for (QObject* o : modules) {
    ReactModuleInterface* module = qobject_cast<ReactModuleInterface*>(o);
    if (module != nullptr) {
      module->setBridge(this);
      ReactModuleData* moduleData = new ReactModuleData(o);
      d->modules.insert(moduleData->id(), moduleData);
    } else {
      qWarning() << "A module loader exported an invalid module";
    }
  }
}

void ReactBridge::injectModules()
{
  Q_D(ReactBridge);

  QVariantMap moduleConfig;

  for (auto& md : d->modules) {
//    qDebug() << "Injecting module" << md->name();
    moduleConfig.insert(md->name(), md->info());
  }

  d->executor->injectJson("__fbBatchedBridgeConfig", QVariantMap{
    { "remoteModuleConfig", moduleConfig }
  });
}

void ReactBridge::processResult(const QJsonDocument& doc)
{
  Q_D(ReactBridge);

  if (doc.isNull())
    return;

  if (!doc.isArray()) {
    qCritical() << "Returned document from executor in unexpected form";
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
    ReactModuleData* moduleData = d->modules[moduleIDs[i].toInt()];
    if (moduleData == nullptr) {
      qCritical() << "Could not find referenced module";
      continue;
    }

    ReactModuleMethod* method = moduleData->method(methodIDs[i].toInt());
    if (method == nullptr) {
      qCritical() << "Request for unsupported method";
      continue;
    }

    method->invokeWithBridge(this, paramArrays[i].toList());
  }
}

void ReactBridge::applicationScriptDone()
{
  QTimer::singleShot(0, [this]() {
      d_func()->executor->executeJSCall("flushedQueue", QVariantList{}, [=](const QJsonDocument& doc) {
          processResult(doc);
          setReady(true);
        });
    });
}
