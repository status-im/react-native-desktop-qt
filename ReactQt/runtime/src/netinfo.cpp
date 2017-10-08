
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

#include "bridge.h"
#include "eventdispatcher.h"
#include "netinfo.h"

namespace {
static QMap<QNetworkAccessManager::NetworkAccessibility, QString> accessibleName{
    {QNetworkAccessManager::UnknownAccessibility, "UnknownAccessibility"},
    {QNetworkAccessManager::NotAccessible, "NotAccessible"},
    {QNetworkAccessManager::Accessible, "Accessible"}};
}

class NetInfoPrivate {
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

    Bridge* bridge = nullptr;
};

void NetInfo::getCurrentConnectivity(const ModuleInterface::ListArgumentBlock& resolve,
                                     const ModuleInterface::ListArgumentBlock& reject) {
    Q_UNUSED(reject);
    Q_D(NetInfo);
    resolve(d->bridge,
            QVariantList{QVariantMap{
                {"network_info", accessibleName.value(d->bridge->networkAccessManager()->networkAccessible())}}});
}

NetInfo::NetInfo(QObject* parent) : QObject(parent), d_ptr(new NetInfoPrivate) {}

NetInfo::~NetInfo() {}

void NetInfo::setBridge(Bridge* bridge) {
    Q_D(NetInfo);
    d->bridge = bridge;
    d->monitorNetworkAccess();
}

QString NetInfo::moduleName() {
    return "RCTNetInfo";
}

QList<ModuleMethod*> NetInfo::methodsToExport() {
    return QList<ModuleMethod*>{};
}

QVariantMap NetInfo::constantsToExport() {
    return QVariantMap{};
}
