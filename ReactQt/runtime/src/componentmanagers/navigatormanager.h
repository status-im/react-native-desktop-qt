
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

#ifndef NAVIGATORMANAGER_H
#define NAVIGATORMANAGER_H

#include <QMap>
#include <QString>

#include "viewmanager.h"

class QQuickItem;

class NavigatorManager : public ViewManager {
    Q_OBJECT
    // Q_PLUGIN_METADATA(IID ReactModuleInterface_IID)
    Q_INTERFACES(ModuleInterface)

    Q_INVOKABLE void push(int containerTag, int viewTag);
    Q_INVOKABLE void pop(int containerTag);
    Q_INVOKABLE void clear(int containerTag);

public:
    NavigatorManager(QObject* parent = 0);
    ~NavigatorManager();

    ViewManager* viewManager() override;

    QString moduleName() override;
    QStringList customBubblingEventTypes() override;

private Q_SLOTS:
    void backTriggered();

private:
    virtual void configureView(QQuickItem* view) const override;
    virtual QString qmlComponentFile(const QVariantMap& properties) const override;

    void invokeMethod(const QString& methodSignature, QQuickItem* item, const QVariantList& args = QVariantList{});
    QMetaMethod findMethod(const QString& methodSignature, QQuickItem* item);

    mutable int m_id;
    QMap<QPair<QString, QQuickItem*>, QMetaMethod> m_methodCache;
};

#endif // NAVIGATORMANAGER_H
