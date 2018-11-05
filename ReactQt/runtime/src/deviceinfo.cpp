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

#include <QGuiApplication>
#include <QQuickItem>
#include <QScreen>

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

    QScreen* screen = QGuiApplication::primaryScreen();

    QQuickItem* rootView = d->bridge->visualParent();
    Q_ASSERT(rootView);
    QVariantMap windowValues{{"fontScale", screen->devicePixelRatio()},
                             {"width", rootView->width()},
                             {"height", rootView->height()},
                             {"scale", screen->devicePixelRatio()}};

    QRect screenGeometry = screen->geometry();
    QVariantMap screenValues{{"fontScale", 8},
                             {"width", screenGeometry.width()},
                             {"height", screenGeometry.height()},
                             {"scale", screen->devicePixelRatio()}};

    QVariantMap values{{"screen", screenValues}, {"window", windowValues}};

    return QVariantMap{{"Dimensions", values}};
}
