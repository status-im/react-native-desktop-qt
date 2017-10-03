
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#ifndef REACTBUTTONMANAGER_H
#define REACTBUTTONMANAGER_H

#include "reactviewmanager.h"

class ReactButtonManagerPrivate;
class ReactButtonManager : public ReactViewManager {
    Q_OBJECT
    Q_INTERFACES(ReactModuleInterface)
    Q_DECLARE_PRIVATE(ReactButtonManager)

public:
    ReactButtonManager(QObject* parent = 0);
    virtual ~ReactButtonManager();

    virtual ReactViewManager* viewManager() override;
    virtual QString moduleName() override;
    virtual QStringList customDirectEventTypes() override;

public slots:
    void sendPressedNotificationToJs(QQuickItem* button);

private:
    virtual QString qmlComponentFile() const override;
    virtual void configureView(QQuickItem* button) const override;

private:
    QScopedPointer<ReactButtonManagerPrivate> d_ptr;
};

#endif // REACTBUTTONMANAGER_H
