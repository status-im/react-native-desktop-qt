
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

#ifndef REACTMODALMANAGER_H
#define REACTMODALMANAGER_H

#include "reactviewmanager.h"

class ReactPropertyHandler;
class ReactModalManagerPrivate;
class ReactModalManager : public ReactViewManager {
    Q_OBJECT
    Q_INTERFACES(ReactModuleInterface)
    Q_DECLARE_PRIVATE(ReactModalManager)

public:
    ReactModalManager(QObject* parent = 0);
    virtual ~ReactModalManager();

    virtual ReactViewManager* viewManager() override;
    virtual QString moduleName() override;
    void addChildItem(QQuickItem* modalView, QQuickItem* child, int position) const override;
    virtual QStringList customDirectEventTypes() override;

public slots:
    void sendOnShowNotificationToJs(QQuickItem* modal);

private:
    virtual QString qmlComponentFile() const override;
    virtual void configureView(QQuickItem* modal) const override;

private:
    QScopedPointer<ReactModalManagerPrivate> d_ptr;
};

#endif // REACTMODALMANAGER_H
