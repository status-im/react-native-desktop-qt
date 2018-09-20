
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
#include <QTimer>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>

#include <memory>

#include "bridge.h"
#include "eventdispatcher.h"
#include "netinfo.h"

namespace {
static QMap<QNetworkAccessManager::NetworkAccessibility, QString> accessibleName{
    {QNetworkAccessManager::UnknownAccessibility, "unknown"},
    {QNetworkAccessManager::NotAccessible, "none"},
    {QNetworkAccessManager::Accessible, "wifi"}};
}

class NetInfoPrivate {
public:
    void monitorNetworkAccess() {


        QObject::connect(bridge->networkAccessManager(),
                         &QNetworkAccessManager::networkAccessibleChanged,
                         [=](QNetworkAccessManager::NetworkAccessibility accessible) {
                             bridge->eventDispatcher()->sendDeviceEvent(
                                 "networkStatusDidChange",
                                 QVariantMap{{"connectionType", accessibleName.value(accessible)}});
                         });

        timer = new QTimer();

        QObject::connect(timer, &QTimer::timeout, 
          [=]() {
              QNetworkRequest req(QUrl("http://www.google.com"));

              // Need to create a fresh instance of QNetworkAccessManager due to
              // https://bugreports.qt.io/browse/QTBUG-49760
              nam = new QNetworkAccessManager();
              QNetworkReply * reply = nam->head(req);
              auto replyFinishOrTimeout = [=]() {
                    auto newAccessible = reply->isFinished() ? 
                                         reply->error() == QNetworkReply::NoError :
                                         false;
                    //qDebug() << "reply->error(): " << reply->error() << "isFinished: " << reply->isFinished(); 
                    auto currentAccessible = 
                      bridge->networkAccessManager()->networkAccessible() == QNetworkAccessManager::Accessible;
                    //qDebug() << "monitorNetworkAccess: " << currentAccessible << " " << newAccessible; 
                    if (newAccessible != currentAccessible) {
                      auto networkAccessible = newAccessible ? 
                                               QNetworkAccessManager::Accessible : 
                                               QNetworkAccessManager::NotAccessible;
                      qDebug() << "monitorNetworkAccess: setNetworkAccessible " 
                               << "old: " << currentAccessible
                               << "new: " << networkAccessible;
                      bridge->networkAccessManager()->setNetworkAccessible(networkAccessible);
                    }
                    reply->deleteLater();
                    //replyTimer->deleteLater();
                    if (replyTimer != nullptr) {
                      replyTimer->stop();
                      delete replyTimer;
                      replyTimer = nullptr;
                    }

                    if (nam != nullptr) {
                      delete nam;
                      nam = nullptr;
                    }
              };

              replyTimer = new QTimer();
              replyTimer->setSingleShot(true);
              QObject::connect(replyTimer, &QTimer::timeout, replyFinishOrTimeout);
              replyTimer->start(1000);
              //QTimer::singleShot(1000, replyFinishOrTimeout);
              //QObject::connect(reply, &QNetworkReply::finished, replyFinishOrTimeout);
        });
        timer->start(1000);


    }

    Bridge* bridge = nullptr;
    QTimer* timer = nullptr;
    QTimer* replyTimer = nullptr;
    QNetworkAccessManager * nam = nullptr;

    ~NetInfoPrivate() {
      if (timer != nullptr) {
        timer->stop();
        delete timer;
      }

      if (replyTimer != nullptr) {
        replyTimer->stop();
        delete replyTimer;
      }

      if (nam != nullptr) {
        delete nam;
      }
    }

};

void NetInfo::getCurrentConnectivity(const ModuleInterface::ListArgumentBlock& resolve,
                                     const ModuleInterface::ListArgumentBlock& reject) {
    Q_UNUSED(reject);
    Q_D(NetInfo);
    resolve(d->bridge,
            QVariantList{QVariantMap{
                {"connectionType", accessibleName.value(d->bridge->networkAccessManager()->networkAccessible())}}});
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

