
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

namespace {
const int NETWORK_REQUEST_TIMEOUT = 3000;
const int NETWORK_REPLY_CHECK_TIMEOUT = 1000;
const int MAX_REPLY_CHECK_COUNTER = NETWORK_REQUEST_TIMEOUT / NETWORK_REPLY_CHECK_TIMEOUT;
} // namespace

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
    void stopNetworkAccessMonitoring();
    QVariantMap networkInfo();
    void setConnectionType(const QString& state);

public slots:
    QString getTypeFromAvailableInterfaces();
    void checkReachability();

public:
    QNetworkConfigurationManager networkManager;
    Bridge* bridge = nullptr;
    QString connectionType = CON_TYPE_NONE;
    QString effectiveConnectionType = EFCON_TYPE_UNKNOWNN;
    int replyStateCheckCounter = 0;
    QTimer requestTimer;
    QTimer replyTimer;
    QString checkUrl = "http://www.google.com";

Q_SIGNALS:
    void networkStateChanged();
};

void NetInfoPrivate::setConnectionType(const QString& type) {
    if (connectionType != type) {
        connectionType = type;
        Q_EMIT networkStateChanged();
    }
}

NetInfoPrivate::NetInfoPrivate() {
    replyTimer.setInterval(NETWORK_REPLY_CHECK_TIMEOUT);
}

NetInfoPrivate::~NetInfoPrivate() {
    requestTimer.stop();
    replyTimer.stop();
}

QVariantMap NetInfoPrivate::networkInfo() {
    return QVariantMap{{"connectionType", connectionType}, {"effectiveConnectionType", effectiveConnectionType}};
}

QString NetInfoPrivate::getTypeFromAvailableInterfaces() {
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
    return type;
}

void NetInfoPrivate::checkReachability() {

    QNetworkRequest req(checkUrl);

    replyStateCheckCounter = 0;

    QNetworkReply* reply = bridge->networkAccessManager()->head(req);
    auto replyFinishOrTimeout = [=]() {
        auto success = reply->isFinished() ? reply->error() == QNetworkReply::NoError : false;
        bool timeout = ++replyStateCheckCounter >= MAX_REPLY_CHECK_COUNTER;

        if (success) {
            setConnectionType(getTypeFromAvailableInterfaces());
        } else if (timeout) {
            setConnectionType(CON_TYPE_NONE);
        }

        if (success || timeout) {
            replyTimer.stop();
            reply->deleteLater();
        }

    };

    QObject::connect(&replyTimer, &QTimer::timeout, reply, replyFinishOrTimeout);
    replyTimer.start();
}

void NetInfoPrivate::startNetworkAccessMonitoring() {

    if (requestTimer.isActive())
        return;

    connect(this, &NetInfoPrivate::networkStateChanged, [=]() {
        if (bridge && bridge->ready()) {
            bridge->eventDispatcher()->sendDeviceEvent("networkStatusDidChange", networkInfo());
        }
    });

    connect(&requestTimer, &QTimer::timeout, this, &NetInfoPrivate::checkReachability);
    requestTimer.start(NETWORK_REQUEST_TIMEOUT);
}

void NetInfoPrivate::stopNetworkAccessMonitoring() {
    requestTimer.stop();
}

void NetInfo::getCurrentConnectivity(const ModuleInterface::ListArgumentBlock& resolve,
                                     const ModuleInterface::ListArgumentBlock& reject) {
    Q_UNUSED(reject);
    Q_D(NetInfo);
    Q_ASSERT(d->bridge);

    resolve(d->bridge, QVariantList{d->networkInfo()});
}

void NetInfo::setConnectionCheckUrl(const QString& url) {
    d_ptr->checkUrl = url;
}

NetInfo::NetInfo(QObject* parent) : QObject(parent), d_ptr(new NetInfoPrivate) {
    d_ptr->startNetworkAccessMonitoring();
}

NetInfo::~NetInfo() {}

void NetInfo::setBridge(Bridge* bridge) {
    Q_D(NetInfo);
    d->bridge = bridge;

    connect(d->bridge, &Bridge::readyChanged, this, [=]() {
        if (d->bridge->ready()) {
            d->startNetworkAccessMonitoring();
        } else {
            d->stopNetworkAccessMonitoring();
        }
    });
}

QString NetInfo::moduleName() {
    return "RCTNetInfo";
}

#include "netinfo.moc"
