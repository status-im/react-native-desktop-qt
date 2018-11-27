
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

#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkConfiguration>
#include <QNetworkConfigurationManager>
#include <QNetworkInterface>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTimer>

#include <memory>

#include "bridge.h"
#include "eventdispatcher.h"
#include "moduleinterface.h"
#include "netinfo.h"

const QString CON_TYPE_NONE = "none";
const QString CON_TYPE_UNKNOWNN = "unknown";
const QString CON_TYPE_WIFI = "wifi";
const QString CON_TYPE_ETHERNET = "ethernet";
const QString EFCON_TYPE_UNKNOWNN = "unknown";

class NetInfoPrivate : public QObject {
    Q_OBJECT
public:
    NetInfoPrivate();
    ~NetInfoPrivate();

    void startNetworkAccessMonitoring();
    QVariantMap networkInfo();
    void setConnectionType(const QString& state);

public slots:
    void checkInterfaces();

public:
    QNetworkConfigurationManager networkManager;
    Bridge* bridge = nullptr;
    QString connectionType = CON_TYPE_NONE;
    QString effectiveConnectionType = EFCON_TYPE_UNKNOWNN;
    int replyStateCheckCounter = 0;

Q_SIGNALS:
    void networkStateChanged();
};

void NetInfoPrivate::setConnectionType(const QString& type) {
    if (connectionType != type) {
        connectionType = type;
        Q_EMIT networkStateChanged();
    }
}

NetInfoPrivate::NetInfoPrivate() {}

NetInfoPrivate::~NetInfoPrivate() {}

QVariantMap NetInfoPrivate::networkInfo() {
    return QVariantMap{{"connectionType", connectionType}, {"effectiveConnectionType", effectiveConnectionType}};
}

void NetInfoPrivate::checkInterfaces() {
    auto type = CON_TYPE_NONE;

    foreach (QNetworkInterface interface, QNetworkInterface::allInterfaces()) {
        bool interfaceUp = interface.flags().testFlag(QNetworkInterface::IsUp);
        bool interfaceNotLoopback = !interface.flags().testFlag(QNetworkInterface::IsLoopBack);
        bool interfaceNotVM = !interface.humanReadableName().contains("VM");
        bool interfaceMacValid = (interface.hardwareAddress() != "00:00:00:00:00:00");

        if (interfaceUp && interfaceNotLoopback && interfaceNotVM)
            foreach (QNetworkAddressEntry entry, interface.addressEntries()) {
                if (interfaceMacValid && entry.ip().toString().contains(".")) {
                    if (interface.type() == QNetworkInterface::Wifi)
                        type = CON_TYPE_WIFI;
                    else if (interface.type() == QNetworkInterface::Ethernet)
                        type = CON_TYPE_ETHERNET;
                }
            }
    }
    setConnectionType(type);
}

void NetInfoPrivate::startNetworkAccessMonitoring() {

    connect(this, &NetInfoPrivate::networkStateChanged, [=]() {
        if (bridge) {
            qDebug() << "!! send event";
            bridge->eventDispatcher()->sendDeviceEvent("networkStatusDidChange", networkInfo());
        }
    });

    connect(&networkManager, &QNetworkConfigurationManager::updateCompleted, this, &NetInfoPrivate::checkInterfaces);
}

void NetInfo::getCurrentConnectivity(const ModuleInterface::ListArgumentBlock& resolve,
                                     const ModuleInterface::ListArgumentBlock& reject) {
    Q_UNUSED(reject);
    Q_D(NetInfo);
    Q_ASSERT(d->bridge);

    resolve(d->bridge, QVariantList{d->networkInfo()});
}

NetInfo::NetInfo(QObject* parent) : QObject(parent), d_ptr(new NetInfoPrivate) {
    d_ptr->startNetworkAccessMonitoring();
}

NetInfo::~NetInfo() {}

void NetInfo::setBridge(Bridge* bridge) {
    Q_D(NetInfo);
    d->bridge = bridge;
}

QString NetInfo::moduleName() {
    return "RCTNetInfo";
}

#include "netinfo.moc"
