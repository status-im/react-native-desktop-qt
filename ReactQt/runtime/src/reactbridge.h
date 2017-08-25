
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

#ifndef REACTBRIDGE_H
#define REACTBRIDGE_H

#include <QUrl>
#include <QObject>
#include <QScopedPointer>


class QQuickItem;
class QQmlEngine;
class QNetworkAccessManager;
class ReactModuleData;
class ReactUIManager;
class ReactImageLoader;
class ReactEventDispatcher;
class ReactRedboxItem;
class ReactTestModule;

class ReactBridgePrivate;
class ReactBridge : public QObject
{
  Q_OBJECT
  Q_PROPERTY(bool ready READ ready WRITE setReady NOTIFY readyChanged)
  Q_PROPERTY(bool jsAppStarted READ jsAppStarted NOTIFY jsAppStartedChanged)
  Q_PROPERTY(QQuickItem* visualParent READ visualParent WRITE setVisualParent)
  Q_PROPERTY(QQmlEngine* qmlEngine READ qmlEngine WRITE setQmlEngine)
  Q_PROPERTY(QNetworkAccessManager* networkAccessManager READ networkAccessManager WRITE setNetworkAccessManager)
  Q_PROPERTY(QUrl bundleUrl READ bundleUrl WRITE setBundleUrl)
  Q_PROPERTY(QString pluginsPath READ pluginsPath WRITE setPluginsPath)
  Q_PROPERTY(QString executorName READ executorName WRITE setExecutorName)
  Q_PROPERTY(QList<ReactModuleData*> modules READ modules)
  Q_PROPERTY(ReactUIManager* uiManager READ uiManager)
  Q_PROPERTY(ReactImageLoader* imageLoader READ imageLoader)

  enum Fields { FieldRequestModuleIDs, FieldMethodIDs, FieldParams };

  Q_DECLARE_PRIVATE(ReactBridge)

public:
  ReactBridge(QObject* parent = 0);
  ~ReactBridge();

  void init();
  void reload();

  void enqueueJSCall(const QString& module, const QString& method, const QVariantList& args);
  void invokeAndProcess(const QString& method, const QVariantList &args);
  void executeSourceCode(const QByteArray& sourceCode);
  void enqueueRunAppCall(const QVariantList& args);

  // XXX: maybe rename
  bool ready() const;
  void setReady(bool ready);

  bool jsAppStarted() const;

  QQuickItem* visualParent() const;
  void setVisualParent(QQuickItem* item);

  QQmlEngine* qmlEngine() const;
  void setQmlEngine(QQmlEngine* qmlEngine);

  QNetworkAccessManager* networkAccessManager() const;
  void setNetworkAccessManager(QNetworkAccessManager* nam);

  QUrl bundleUrl() const;
  void setBundleUrl(const QUrl& bundleUrl);

  QString pluginsPath() const;
  void setPluginsPath(const QString& pluginsPath);

  QString executorName() const;
  void setExecutorName(const QString& executorName);

  ReactEventDispatcher* eventDispatcher() const;
  QList<ReactModuleData*> modules() const;
  ReactUIManager* uiManager() const;
  ReactTestModule* testModule() const;
  ReactImageLoader* imageLoader() const;
  ReactRedboxItem* redbox();

Q_SIGNALS:
  void readyChanged();
  void jsAppStartedChanged();

private Q_SLOTS:
  void sourcesFinished();
  void sourcesLoadFailed();
  void applicationScriptDone();

private:
  void loadSource();
  void initModules();
  void injectModules();
  void processResult(const QJsonDocument& document);
  void setupExecutor();
  void setJsAppStarted(bool started);
  void invokeModuleMethod(int moduleId, int methodId, QList<QVariant> args);

  QScopedPointer<ReactBridgePrivate> d_ptr;
};

#endif // REACTBRIDGE_H
