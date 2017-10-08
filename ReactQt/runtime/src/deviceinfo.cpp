/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include "deviceinfo.h"
#include "bridge.h"
#include "eventdispatcher.h"

class DeviceInfoPrivate {
public:
    Bridge* bridge = nullptr;
};

DeviceInfo::DeviceInfo(QObject* parent) : QObject(parent), d_ptr(new DeviceInfoPrivate) {}

DeviceInfo::~DeviceInfo() {}

void DeviceInfo::setBridge(Bridge* bridge) {
    Q_D(DeviceInfo);
    d->bridge = bridge;
}

QString DeviceInfo::moduleName() {
    return "RCTDeviceInfo";
}

QList<ModuleMethod*> DeviceInfo::methodsToExport() {
    return QList<ModuleMethod*>{};
}

QVariantMap DeviceInfo::constantsToExport() {
    // TODO: Populate Dimensions with real data
    QVariantMap values{{"fontScale", 1}, {"width", 375}, {"height", 667}, {"scale", 2}};
    QVariantMap screenValues{{"screen", values}, {"window", values}};

    return QVariantMap{{"Dimensions", screenValues}};
}
