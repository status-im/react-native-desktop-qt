/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include "reactdeviceinfo.h"
#include "reactbridge.h"
#include "reacteventdispatcher.h"

class ReactDeviceInfoPrivate {
public:
    ReactBridge* bridge = nullptr;
};

ReactDeviceInfo::ReactDeviceInfo(QObject* parent) : QObject(parent), d_ptr(new ReactDeviceInfoPrivate) {}

ReactDeviceInfo::~ReactDeviceInfo() {}

void ReactDeviceInfo::setBridge(ReactBridge* bridge) {
    Q_D(ReactDeviceInfo);
    d->bridge = bridge;
}

QString ReactDeviceInfo::moduleName() {
    return "RCTDeviceInfo";
}

QList<ReactModuleMethod*> ReactDeviceInfo::methodsToExport() {
    return QList<ReactModuleMethod*>{};
}

QVariantMap ReactDeviceInfo::constantsToExport() {
    // TODO: Populate Dimensions with real data
    QVariantMap values{{"fontScale", 1}, {"width", 375}, {"height", 667}, {"scale", 2}};
    QVariantMap screenValues{{"screen", values}, {"window", values}};

    return QVariantMap{{"Dimensions", screenValues}};
}
