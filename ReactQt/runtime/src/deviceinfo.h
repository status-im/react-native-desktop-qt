/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include "moduleinterface.h"

#include <QVariantMap>

class DeviceInfoPrivate;
class DeviceInfo : public QObject, public ModuleInterface {
    Q_OBJECT
    Q_INTERFACES(ModuleInterface)

    Q_DECLARE_PRIVATE(DeviceInfo)

public:
    DeviceInfo(QObject* parent = 0);
    ~DeviceInfo();

    void setBridge(Bridge* bridge) override;

    QString moduleName() override;
    QList<ModuleMethod*> methodsToExport() override;
    QVariantMap constantsToExport() override;

private:
    QScopedPointer<DeviceInfoPrivate> d_ptr;
};

#endif // DEVICEINFO_H
