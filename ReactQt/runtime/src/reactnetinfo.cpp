
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

#include <QNetworkAccessManager>

#include "reactbridge.h"
#include "reacteventdispatcher.h"
#include "reactnetinfo.h"

namespace {
static QMap<QNetworkAccessManager::NetworkAccessibility, QString> accessibleName{
    {QNetworkAccessManager::UnknownAccessibility, "UnknownAccessibility"},
    {QNetworkAccessManager::NotAccessible, "NotAccessible"},
    {QNetworkAccessManager::Accessible, "Accessible"}};
}

class ReactNetInfoPrivate {
public:
    void monitorNetworkAccess() {
        QObject::connect(bridge->networkAccessManager(),
                         &QNetworkAccessManager::networkAccessibleChanged,
                         [=](QNetworkAccessManager::NetworkAccessibility accessible) {
                             bridge->eventDispatcher()->sendDeviceEvent(
                                 "networkStatusDidChange",
                                 QVariantList{QVariantMap{{"network_info", accessibleName.value(accessible)}}});
                         });
    }

    ReactBridge* bridge = nullptr;
};

void ReactNetInfo::getCurrentConnectivity(const ReactModuleInterface::ListArgumentBlock& resolve,
                                          const ReactModuleInterface::ListArgumentBlock& reject) {
    Q_UNUSED(reject);
    Q_D(ReactNetInfo);
    resolve(d->bridge,
            QVariantList{QVariantMap{
                {"network_info", accessibleName.value(d->bridge->networkAccessManager()->networkAccessible())}}});
}

ReactNetInfo::ReactNetInfo(QObject* parent) : QObject(parent), d_ptr(new ReactNetInfoPrivate) {}

ReactNetInfo::~ReactNetInfo() {}

void ReactNetInfo::setBridge(ReactBridge* bridge) {
    Q_D(ReactNetInfo);
    d->bridge = bridge;
    d->monitorNetworkAccess();
}

QString ReactNetInfo::moduleName() {
    return "RCTNetInfo";
}

QList<ReactModuleMethod*> ReactNetInfo::methodsToExport() {
    return QList<ReactModuleMethod*>{};
}

QVariantMap ReactNetInfo::constantsToExport() {
    return QVariantMap{};
}
