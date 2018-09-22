
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
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTimer>

#include <memory>

#include "bridge.h"
#include "eventdispatcher.h"
#include "netinfo.h"

class NetInfoPrivate : public QObject {
    Q_OBJECT
public:
    enum NetworkState { UnkownState = 0, NotAvailable, Available };

    ~NetInfoPrivate();
    void monitorNetworkAccess();

    void setNetworkState(NetworkState state);

    Bridge* bridge = nullptr;
    QTimer timer;
    QTimer* replyTimer = nullptr;
    NetworkState networkState = UnkownState;

Q_SIGNALS:
    void networkStateChanged();
};

namespace {
static QMap<NetInfoPrivate::NetworkState, QString> accessibleName{{NetInfoPrivate::UnkownState, "unknown"},
                                                                  {NetInfoPrivate::NotAvailable, "none"},
                                                                  {NetInfoPrivate::Available, "wifi"}};
} // namespace

void NetInfoPrivate::setNetworkState(NetInfoPrivate::NetworkState state) {
    if (networkState != state) {
        networkState = state;
        Q_EMIT networkStateChanged();
    }
}

NetInfoPrivate::~NetInfoPrivate() {

    timer.stop();

    if (replyTimer != nullptr) {
        replyTimer->stop();
        delete replyTimer;
    }
}
void NetInfoPrivate::monitorNetworkAccess() {
    QObject::connect(this, &NetInfoPrivate::networkStateChanged, [=]() {
        bridge->eventDispatcher()->sendDeviceEvent("networkStatusDidChange",
                                                   QVariantMap{{"connectionType", accessibleName.value(networkState)}});
    });

    QObject::connect(&timer, &QTimer::timeout, [=]() {
        QNetworkRequest req(QUrl("http://www.google.com"));

        QNetworkReply* reply = bridge->networkAccessManager()->head(req);
        auto replyFinishOrTimeout = [=]() {
            auto replyFinishedNoError = reply->isFinished() ? reply->error() == QNetworkReply::NoError : false;
            auto previousNetworkState = (networkState == NetInfoPrivate::Available);
            if (replyFinishedNoError != previousNetworkState) {
                auto newNetworkState = replyFinishedNoError ? NetInfoPrivate::Available : NetInfoPrivate::NotAvailable;
                qDebug() << "monitorNetworkAccess: setNetworkAccessible "
                         << "old: " << previousNetworkState << "new: " << newNetworkState;
                setNetworkState(newNetworkState);
            }
            reply->deleteLater();
            if (replyTimer != nullptr) {
                replyTimer->stop();
                delete replyTimer;
                replyTimer = nullptr;
            }
        };

        replyTimer = new QTimer();
        replyTimer->setSingleShot(true);
        QObject::connect(replyTimer, &QTimer::timeout, this, replyFinishOrTimeout);
        replyTimer->start(1000);
    });
    timer.start(1000);
}
void NetInfo::getCurrentConnectivity(const ModuleInterface::ListArgumentBlock& resolve,
                                     const ModuleInterface::ListArgumentBlock& reject) {
    Q_UNUSED(reject);
    Q_D(NetInfo);
    resolve(d->bridge, QVariantList{QVariantMap{{"connectionType", accessibleName.value(d->networkState)}}});
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

#include "netinfo.moc"
