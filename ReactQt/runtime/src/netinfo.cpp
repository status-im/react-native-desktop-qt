
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

namespace {

const int NETWORK_REQUEST_TIMEOUT = 3000;
const int NETWORK_REPLY_CHECK_TIMEOUT = 1000;
const int MAX_REPLY_CHECK_COUNTER = NETWORK_REQUEST_TIMEOUT / NETWORK_REPLY_CHECK_TIMEOUT;
} // namespace

class NetInfoPrivate : public QObject {
    Q_OBJECT
public:
    enum NetworkState { UnkownState = 0, NotAvailable, Available };

    NetInfoPrivate();
    ~NetInfoPrivate();
    void monitorNetworkAccess();

    void setNetworkState(NetworkState state);

    Bridge* bridge = nullptr;
    QTimer requestTimer;
    QTimer replyTimer;
    NetworkState networkState = UnkownState;
    int replyStateCheckCounter = 0;

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

NetInfoPrivate::NetInfoPrivate() {
    replyTimer.setInterval(NETWORK_REPLY_CHECK_TIMEOUT);
}

NetInfoPrivate::~NetInfoPrivate() {
    requestTimer.stop();
    replyTimer.stop();
}

void NetInfoPrivate::monitorNetworkAccess() {
    if (requestTimer.isActive())
        return;
    QObject::connect(this, &NetInfoPrivate::networkStateChanged, [=]() {
        bridge->eventDispatcher()->sendDeviceEvent("networkStatusDidChange",
                                                   QVariantMap{{"connectionType", accessibleName.value(networkState)}});
    });

    QObject::connect(&requestTimer, &QTimer::timeout, [=]() {
        QNetworkRequest req(QUrl("http://www.google.com"));

        replyStateCheckCounter = 0;
        replyTimer.start();

        QNetworkReply* reply = bridge->networkAccessManager()->head(req);
        auto replyFinishOrTimeout = [=]() {
            auto replyFinishedNoError = reply->isFinished() ? reply->error() == QNetworkReply::NoError : false;
            auto previousNetworkState = (networkState == NetInfoPrivate::Available);

            if (++replyStateCheckCounter >= MAX_REPLY_CHECK_COUNTER || replyFinishedNoError) {
                replyTimer.stop();

                if (replyFinishedNoError != previousNetworkState) {
                    auto newNetworkState =
                        replyFinishedNoError ? NetInfoPrivate::Available : NetInfoPrivate::NotAvailable;
                    qDebug() << "monitorNetworkAccess: setNetworkAccessible "
                             << "old: " << previousNetworkState << "new: " << newNetworkState;
                    setNetworkState(newNetworkState);
                }
                reply->deleteLater();
            }
        };

        QObject::connect(&replyTimer, &QTimer::timeout, reply, replyFinishOrTimeout);
        replyTimer.start();
    });

    requestTimer.start(NETWORK_REQUEST_TIMEOUT);
}
void NetInfo::getCurrentConnectivity(const ModuleInterface::ListArgumentBlock& resolve,
                                     const ModuleInterface::ListArgumentBlock& reject) {
    Q_UNUSED(reject);
    Q_D(NetInfo);
}

NetInfo::NetInfo(QObject* parent) : QObject(parent), d_ptr(new NetInfoPrivate) {}

NetInfo::~NetInfo() {}

void NetInfo::setBridge(Bridge* bridge) {
    Q_D(NetInfo);
    d->bridge = bridge;

    connect(d->bridge, &Bridge::jsAppStartedChanged, this, [=]() {
        if (d->bridge->jsAppStarted()) {
            d->monitorNetworkAccess();
        }
    });
}

QString NetInfo::moduleName() {
    return "RCTNetInfo";
}

#include "netinfo.moc"
