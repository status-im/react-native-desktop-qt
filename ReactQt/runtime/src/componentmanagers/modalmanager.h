
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

#ifndef MODALMANAGER_H
#define MODALMANAGER_H

#include "viewmanager.h"

class PropertyHandler;
class ModalManagerPrivate;
class ModalManager : public ViewManager {
    Q_OBJECT
    Q_INTERFACES(ModuleInterface)
    Q_DECLARE_PRIVATE(ModalManager)

public:
    ModalManager(QObject* parent = 0);
    virtual ~ModalManager();

    virtual ViewManager* viewManager() override;
    virtual QString moduleName() override;
    void addChildItem(QQuickItem* modalView, QQuickItem* child, int position) const override;
    virtual QStringList customDirectEventTypes() override;

public slots:
    void sendOnShowNotificationToJs(QQuickItem* modal);

private:
    virtual QString qmlComponentFile(const QVariantMap& properties) const override;
    virtual void configureView(QQuickItem* modal) const override;

private:
    QScopedPointer<ModalManagerPrivate> d_ptr;
};

#endif // MODALMANAGER_H
