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

#include <QQuickItem>

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
    Q_D(DeviceInfo);

    // TODO: Populate Dimensions with real data
    QQuickItem* rootView = d->bridge->visualParent();
    Q_ASSERT(rootView);
    QVariantMap values{{"fontScale", 8}, {"width", rootView->width()}, {"height", rootView->height()}, {"scale", 8}};
    QVariantMap screenValues{{"screen", values}, {"window", values}};

    return QVariantMap{{"Dimensions", screenValues}};
}
