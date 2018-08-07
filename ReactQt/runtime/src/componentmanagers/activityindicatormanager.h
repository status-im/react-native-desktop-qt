
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#ifndef ACTIVITYINDICATORMANAGER_H
#define ACTIVITYINDICATORMANAGER_H

#include "viewmanager.h"

class PropertyHandler;
class ActivityIndicatorManagerPrivate;
class ActivityIndicatorManager : public ViewManager {
    Q_OBJECT
    Q_INTERFACES(ModuleInterface)
    Q_DECLARE_PRIVATE(ActivityIndicatorManager)

public:
    ActivityIndicatorManager(QObject* parent = 0);
    virtual ~ActivityIndicatorManager();

    virtual ViewManager* viewManager() override;
    virtual QString moduleName() override;

private:
    virtual QString qmlComponentFile(const QVariantMap& properties) const override;

private:
    QScopedPointer<ActivityIndicatorManagerPrivate> d_ptr;
};

#endif // ACTIVITYINDICATORMANAGER_H
