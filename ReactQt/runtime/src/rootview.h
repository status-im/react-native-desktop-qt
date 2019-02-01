
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

#ifndef ROOTVIEW_H
#define ROOTVIEW_H

#include <QScopedPointer>
#include <QString>
#include <QUrl>

#include "reactitem.h"

class Bridge;

class RootViewPrivate;
class RootView : public ReactItem {
    Q_OBJECT

    Q_PROPERTY(bool liveReload READ liveReload WRITE setLiveReload NOTIFY liveReloadChanged)
    Q_PROPERTY(bool hotReload READ hotReload WRITE setHotReload NOTIFY hotReloadChanged)
    Q_PROPERTY(QString moduleName READ moduleName WRITE setModuleName NOTIFY moduleNameChanged)
    Q_PROPERTY(QUrl codeLocation READ codeLocation WRITE setCodeLocation NOTIFY codeLocationChanged)
    Q_PROPERTY(QVariantMap properties READ properties WRITE setProperties NOTIFY propertiesChanged)
    Q_PROPERTY(QString pluginsPath READ pluginsPath WRITE setPluginsPath NOTIFY pluginsPathChanged)
    Q_PROPERTY(QString jsExecutor READ jsExecutor WRITE setJsExecutor NOTIFY jsExecutorChanged)
    Q_PROPERTY(
        QString serverConnectionType READ serverConnectionType WRITE setServerConnectionType NOTIFY executorChanged)
    Q_PROPERTY(QVariantList externalModules READ externalModules WRITE setExternalModules NOTIFY externalModulesChanged)

    Q_DECLARE_PRIVATE(RootView)

public:
    RootView(QQuickItem* parent = 0);
    ~RootView();

    bool liveReload() const;
    void setLiveReload(bool liveReload);

    bool hotReload() const;
    void setHotReload(bool hotReload);

    QString moduleName() const;
    void setModuleName(const QString& moduleName);

    QUrl codeLocation() const;
    void setCodeLocation(const QUrl& codeLocation);

    QVariantMap properties() const;
    void setProperties(const QVariantMap& properties);

    QString pluginsPath() const;
    void setPluginsPath(const QString& pluginsPath);

    QString jsExecutor() const;
    void setJsExecutor(const QString& jsExecutor);

    QString serverConnectionType() const;
    void setServerConnectionType(const QString& serverConnectionType);

    QVariantList externalModules() const;
    void setExternalModules(const QVariantList& externalModules);

    Bridge* bridge() const;

    void loadBundle(const QString& moduleName, const QUrl& codeLocation);

    Q_INVOKABLE void startRemoteJSDebugging();
    Q_INVOKABLE void reloadBridge();

Q_SIGNALS:
    void liveReloadChanged();
    void hotReloadChanged();
    void moduleNameChanged();
    void codeLocationChanged();
    void propertiesChanged();
    void pluginsPathChanged();
    void executorChanged();
    void externalModulesChanged();
    void jsExecutorChanged();

protected:
    virtual void updatePolish() override;

private Q_SLOTS:
    void bridgeReady();
    void requestPolish();
    void onSizeChanged();
    void sendSizeUpdate();

private:
    void componentComplete() override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    bool childMouseEventFilter(QQuickItem* item, QEvent* event) override;
    void sendMouseEvent(QMouseEvent* event, const QString& eventType, QQuickItem* receiver);
#ifdef RCT_DEV
    void loadDevMenu();
#endif // RCT_DEV

    QScopedPointer<RootViewPrivate> d_ptr;
};

#endif // ROOTVIEW_H
