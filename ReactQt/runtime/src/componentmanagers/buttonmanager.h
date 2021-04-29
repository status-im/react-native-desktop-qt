
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#ifndef BUTTONMANAGER_H
#define BUTTONMANAGER_H

#include "viewmanager.h"

class ButtonManagerPrivate;
class ButtonManager : public ViewManager {
    Q_OBJECT
    Q_INTERFACES(ModuleInterface)
    Q_DECLARE_PRIVATE(ButtonManager)

public:
    ButtonManager(QObject* parent = 0);
    virtual ~ButtonManager();

    virtual ViewManager* viewManager() override;
    virtual QString moduleName() override;
    virtual QStringList customDirectEventTypes() override;
    virtual QStringList customBubblingEventTypes() override;

public slots:
    void sendPressedNotificationToJs(QQuickItem* button);

private:
    virtual QString qmlComponentFile(const QVariantMap& properties) const override;
    virtual void configureView(QQuickItem* button) const override;

private:
    QScopedPointer<ButtonManagerPrivate> d_ptr;
};

#endif // BUTTONMANAGER_H
