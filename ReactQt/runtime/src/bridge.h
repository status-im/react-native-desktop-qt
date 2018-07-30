
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

#ifndef BRIDGE_H
#define BRIDGE_H

#include <QObject>
#include <QScopedPointer>
#include <QUrl>

#include <functional>

class QQuickItem;
class QQmlEngine;
class QNetworkAccessManager;
class ModuleData;
class UIManager;
class ImageLoader;
class EventDispatcher;
class Redbox;
class TestModule;
class ModuleInterface;

class BridgePrivate;
class Bridge : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool ready READ ready WRITE setReady NOTIFY readyChanged)
    Q_PROPERTY(bool jsAppStarted READ jsAppStarted NOTIFY jsAppStartedChanged)
    Q_PROPERTY(QQuickItem* visualParent READ visualParent WRITE setVisualParent)
    Q_PROPERTY(QQmlEngine* qmlEngine READ qmlEngine WRITE setQmlEngine)
    Q_PROPERTY(QNetworkAccessManager* networkAccessManager READ networkAccessManager WRITE setNetworkAccessManager)
    Q_PROPERTY(QUrl bundleUrl READ bundleUrl WRITE setBundleUrl)
    Q_PROPERTY(QString pluginsPath READ pluginsPath WRITE setPluginsPath)
    Q_PROPERTY(QString serverConnectionType READ serverConnectionType WRITE setServerConnectionType)
    Q_PROPERTY(QList<ModuleData*> modules READ modules)
    Q_PROPERTY(UIManager* uiManager READ uiManager)
    Q_PROPERTY(ImageLoader* imageLoader READ imageLoader)

    enum Fields { FieldRequestModuleIDs, FieldMethodIDs, FieldParams };

    Q_DECLARE_PRIVATE(Bridge)

public:
    Bridge(QObject* parent = 0);
    ~Bridge();

    void init();
    void reload();
    void loadBundle(const QUrl& bundleUrl);
    void reset();

    void invokePromiseCallback(double callbackCode, const QVariantList& args);
    void enqueueJSCall(const QString& module, const QString& method, const QVariantList& args);
    void invokeAndProcess(const QString& method, const QVariantList& args);
    void executeSourceCode(const QByteArray& sourceCode);
    void enqueueRunAppCall(const QVariantList& args);

    // XXX: maybe rename
    bool ready() const;
    void setReady(bool ready);

    bool jsAppStarted() const;

    QQuickItem* visualParent() const;
    void setVisualParent(QQuickItem* item);

    QQuickItem* topmostVisualParent() const;

    QQmlEngine* qmlEngine() const;
    void setQmlEngine(QQmlEngine* qmlEngine);

    QNetworkAccessManager* networkAccessManager() const;
    void setNetworkAccessManager(QNetworkAccessManager* nam);

    QUrl bundleUrl() const;
    void setBundleUrl(const QUrl& bundleUrl);

    QString pluginsPath() const;
    void setPluginsPath(const QString& pluginsPath);

    QString serverConnectionType() const;
    void setServerConnectionType(const QString& serverConnectionType);

    const QVariantList& externalModules() const;
    void setExternalModules(const QVariantList& externalModules);

    EventDispatcher* eventDispatcher() const;
    QList<ModuleData*> modules() const;
    UIManager* uiManager() const;
    TestModule* testModule() const;
    ImageLoader* imageLoader() const;
    Redbox* redbox();

    void setRemoteJSDebugging(bool value);

    void setHotReload(bool value);

    void partialBatchDidFlush();
    void batchDidComplete();

    void* getJavaScriptContext();
    void executeOnJavaScriptThread(std::function<void()> func);

    void loadSource();
    void initModules();

Q_SIGNALS:
    void readyChanged();
    void jsAppStartedChanged();

private Q_SLOTS:
    void sourcesFinished();
    void sourcesLoadFailed();
    void applicationScriptDone();

private:
    void loadExternalModules(QObjectList* modules);
    void injectModules();
    void processResult(const QJsonDocument& document);
    void setupExecutor();
    void resetExecutor();
    void setJsAppStarted(bool started);
    Q_INVOKABLE void invokeModuleMethod(int moduleId, int methodId, QList<QVariant> args);
    void addModuleData(QObject* module);

    QScopedPointer<BridgePrivate> d_ptr;
};

#endif // BRIDGE_H
