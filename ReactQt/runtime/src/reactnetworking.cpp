
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

#include <QMetaObject>
#include <QMetaMethod>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <QDebug>

#include "reactnetworking.h"
#include "reactbridge.h"
#include "reacteventdispatcher.h"


namespace {
QVariantMap headerListToMap(const QList<QNetworkReply::RawHeaderPair>& rawHeaderPairs) {
  QVariantMap rh;
  for (auto& hp : rawHeaderPairs) {
    rh.insert(hp.first, hp.second);
  }
  return rh;
}
}


class ReactNetworkingPrivate {
public:
  ReactBridge* bridge;
  QNetworkAccessManager* nam;
  QMap<int, QNetworkReply*> activeConnections;

  void handleGetRequest(int requestId, const QNetworkRequest& request) {
    QNetworkReply* reply = nam->get(request);
    QObject::connect(reply, &QNetworkReply::metaDataChanged, [=]() {
      bridge->eventDispatcher()->sendDeviceEvent("didReceiveNetworkResponse", QVariantList{
        requestId,
        reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(),
        headerListToMap(reply->rawHeaderPairs()),
        reply->url().toString()
      });
    });
    QObject::connect(reply, &QNetworkReply::downloadProgress, [=](qint64 bytesReceived, qint64 bytesTotal) {
      if (bytesReceived == 0) {
        return;
      }
      bridge->eventDispatcher()->sendDeviceEvent("didReceiveNetworkData", QVariantList{
        requestId,
        reply->read(bytesReceived)
      });
    });
    QObject::connect(reply, &QNetworkReply::finished, [=]() {
      reply->deleteLater();
      bridge->eventDispatcher()->sendDeviceEvent("didCompleteNetworkResponse", QVariantList{
        requestId,
        reply->error() != QNetworkReply::NoError ? QVariant(reply->errorString()) : QVariant()
      });
      activeConnections.remove(requestId);
    });
    activeConnections[requestId] = reply;
  }
};


void ReactNetworking::sendRequest(
  int requestId,
  const QString& method,
  const QUrl& url,
  const QVariantMap& headers,
  const QByteArray& data
) {
  // qDebug() << __PRETTY_FUNCTION__ << method << url;
  Q_D(ReactNetworking);

  QNetworkRequest request(url);

  for (const QString& key : headers.keys()) {
    request.setRawHeader(key.toLocal8Bit(), headers[key].toString().toLocal8Bit());
  }

  // TODO: data (post etc)

  if (method.compare("get", Qt::CaseInsensitive) == 0) {
    d->handleGetRequest(requestId, request);
  }
}

void ReactNetworking::abortRequest(int requestId)
{
  // qDebug() << __PRETTY_FUNCTION__ << request;
  Q_D(ReactNetworking);
  QNetworkReply* reply = d->activeConnections[requestId];
  if (reply == nullptr) {
    qCritical() << __PRETTY_FUNCTION__ << "Abort failed; no relevant QNetworkReply found";
    return;
  }
  reply->abort();
  reply->deleteLater();
  d->activeConnections.remove(requestId);
}


ReactNetworking::ReactNetworking(QObject* parent)
  : QObject(parent)
  , d_ptr(new ReactNetworkingPrivate)
{
  Q_D(ReactNetworking);
  d->nam = nullptr;
}

ReactNetworking::~ReactNetworking()
{
}

void ReactNetworking::setBridge(ReactBridge* bridge)
{
  Q_D(ReactNetworking);
  d->bridge = bridge;
  d->nam = bridge->networkAccessManager();
}

ReactViewManager* ReactNetworking::viewManager()
{
  return nullptr;
}

QString ReactNetworking::moduleName()
{
  return "RCTNetworking";
}

QList<ReactModuleMethod*> ReactNetworking::methodsToExport()
{
  return QList<ReactModuleMethod*>{};
}

QVariantMap ReactNetworking::constantsToExport()
{
  return QVariantMap{};
}
