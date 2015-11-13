

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QPluginLoader>
#include <QJsonDocument>
#include <QTimer>

#include "reactbridge.h"
#include "reactnetexecutor.h"
#include "reactmoduleinterface.h"
#include "reactmoduledata.h"
#include "reactmodulemethod.h"

#include "ubuntuuimanager.h"
#include "ubuntuviewmanager.h"
#include "ubunturawtextmanager.h"
#include "ubuntutextmanager.h"
#include "ubuntuactivitymanager.h"
#include "ubuntuscrollviewmanager.h"
//#include "ubuntudatepickermanager.h"
#include "reactnetworking.h"
#include "reacttiming.h"

// Q_IMPORT_PLUGIN(UbuntuUIManager)
// Q_IMPORT_PLUGIN(UbuntuViewManager)
// Q_IMPORT_PLUGIN(UbuntuActivityManager)
// Q_IMPORT_PLUGIN(ReactNetworking)


ReactBridge::ReactBridge(QObject* parent)
  : QObject(parent)
  , m_executor(new ReactNetExecutor(this))
  , m_nam(0)
{
  connect(m_executor, SIGNAL(applicationScriptDone()), SLOT(applicationScriptDone()));
  connect(m_executor, SIGNAL(javascriptCallDone(QVariant)), SLOT(javascriptCallDone(QVariant)));
}

ReactBridge::~ReactBridge()
{
}

void ReactBridge::init()
{
  qDebug() << __func__;

  m_executor->init();

  initModules();

  loadSource();
  // execute source code
}


void ReactBridge::enqueueJSCall
(
  const QString& module,
  const QString& method,
  const QVariantList& args
)
{
  m_executor->executeJSCall("BatchedBridge",
                            "callFunctionReturnFlushedQueue",
                            QVariantList{module, method, args},
                            [=](const QJsonDocument& doc) {
                              processResult(doc);
                            });
}

void ReactBridge::executeSourceCode(const QByteArray& sourceCode)
{
  Q_UNUSED(sourceCode);
}

QQuickItem* ReactBridge::visualParent() const
{
  return m_visualParent;
}

void ReactBridge::setVisualParent(QQuickItem* item)
{
  m_visualParent = item;
}

QQmlEngine* ReactBridge::qmlEngine() const
{
  return m_qmlEngine;
}

void ReactBridge::setQmlEngine(QQmlEngine* qmlEngine)
{
  if (m_qmlEngine == qmlEngine)
    return;

  m_qmlEngine = qmlEngine;
}

QNetworkAccessManager* ReactBridge::networkAccessManager() const
{
  return m_nam;
}

void ReactBridge::setNetworkAccessManager(QNetworkAccessManager* nam)
{
  if (m_nam != nam) {
    m_nam = nam;
  }
}

QUrl ReactBridge::bundleUrl() const
{
  return m_bundleUrl;
}

void ReactBridge::setBundleUrl(const QUrl& bundleUrl)
{
  if (m_bundleUrl != bundleUrl) {
    m_bundleUrl = bundleUrl;
  }
}

QList<ReactModuleData*> ReactBridge::modules() const
{
  return m_modules.values();
}

void ReactBridge::sourcesFinished()
{
  qDebug() << __PRETTY_FUNCTION__;
  QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
  if (reply == 0) {
    qCritical() << "Did not receive a QNetworkReply!";
  }

  if (reply->error() != QNetworkReply::NoError) {
    qCritical() << "Error while loading source";
  }

  m_sourceCode = reply->readAll();
  reply->deleteLater();

  // XXX:
  QTimer::singleShot(200, [this]() {
      m_executor->executeApplicationScript(m_sourceCode, m_bundleUrl);
    });
}

void ReactBridge::loadSource()
{
  qDebug() << __PRETTY_FUNCTION__;

  if (m_nam == 0) {
    qCritical() << "No QNetworkAccessManager for loading sources";
  }

  QNetworkRequest request(m_bundleUrl);
  QNetworkReply* reply = m_nam->get(request);
  connect(reply, SIGNAL(finished()), SLOT(sourcesFinished()));
}


void ReactBridge::initModules()
{
  qDebug() << __func__;
  QVariantMap config;
  QVariantMap moduleConfig;

  // QObjectList modules = QPluginLoader::staticInstances();
  QObjectList modules;
  modules << new UbuntuViewManager;
  modules << new UbuntuRawTextManager;
  modules << new UbuntuTextManager;
  modules << new UbuntuActivityManager;
  modules << new UbuntuScrollViewManager;
  //  modules << new UbuntuDatePickerManager;
  modules << new ReactNetworking;
  modules << new ReactTiming;
  modules << new UbuntuUIManager;

  // XXX:
  Q_FOREACH(QObject* o, modules) {
    ReactModuleInterface* module = qobject_cast<ReactModuleInterface*>(o);
    if (module != nullptr) {
      module->setBridge(this);
      ReactModuleData* moduleData = new ReactModuleData(o);
      m_modules.insert(moduleData->id(), moduleData);
      qDebug() << "Added module" << moduleData->name() << moduleData->id();
      moduleConfig.insert(moduleData->name(), moduleData->info());
    }
  }

  config.insert("remoteModuleConfig", moduleConfig);

  QJsonDocument doc = QJsonDocument::fromVariant(config);
  qDebug() << doc.toJson();

  m_executor->injectJson("__fbBatchedBridgeConfig", config);
}

void ReactBridge::processResult(const QJsonDocument& doc)
{
//  qDebug() << __PRETTY_FUNCTION__ << doc << m_modules;
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
    ReactModuleData* moduleData = m_modules[moduleIDs[i].toInt()];
    if (moduleData == nullptr) {
      qCritical() << "Could not find referenced module";
      continue;
    }

    // TODO; work on argument coercion
    ReactModuleMethod* method = moduleData->method(methodIDs[i].toInt());
    method->invokeWithBridge(this, paramArrays[i].toList());
  }
}

void ReactBridge::applicationScriptDone()
{
  // XXX
  QTimer::singleShot(200, [this]() {
      m_executor->executeJSCall("BatchedBridge", "flushedQueue", QVariantList{}, [=](const QJsonDocument& doc) {
          processResult(doc);
          Q_EMIT bridgeReady();
        });
    });
}

void ReactBridge::javascriptCallDone(const QVariant& result)
{
  Q_UNUSED(result);
}
