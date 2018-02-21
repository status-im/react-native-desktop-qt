
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

#include "viewmanager.h"

class PropertyHandler;
class CameraManagerPrivate;
class CameraManager : public ViewManager {
    Q_OBJECT
    Q_INTERFACES(ModuleInterface)
    Q_DECLARE_PRIVATE(CameraManager)

public:
    CameraManager(QObject* parent = 0);
    virtual ~CameraManager();

    virtual ViewManager* viewManager() override;
    virtual QString moduleName() override;
    virtual QVariantMap constantsToExport() override;

private:
    virtual QString qmlComponentFile() const override;
    virtual void configureView(QQuickItem* view) const override;

private:
    QScopedPointer<CameraManagerPrivate> d_ptr;
};

#endif // CAMERAMANAGER_H
