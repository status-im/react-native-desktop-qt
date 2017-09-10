
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

#ifndef REACTTOUCHABLEMANAGER_H
#define REACTTOUCHABLEMANAGER_H

#include "reactviewmanager.h"

// #define QT_STATICPLUGIN

class ReactTouchableManager : public ReactViewManager {
    Q_OBJECT
    Q_INTERFACES(ReactModuleInterface)

public:
    ReactTouchableManager(QObject* parent = 0);
    ~ReactTouchableManager();

    void setBridge(ReactBridge* bridge) override;

    ReactViewManager* viewManager() override;

    QString moduleName() override;
    QList<ReactModuleMethod*> methodsToExport() override;
    QVariantMap constantsToExport() override;

    QQuickItem* view(const QVariantMap& properties) const override;

    void applyProperties(QQuickItem* item, const QVariantMap& properties) const override;

private Q_SLOTS:
    void clicked();

private:
    void configureView(QQuickItem* view) const;

    int m_id;
};

#endif // REACTTOUCHABLEMANAGER_H
