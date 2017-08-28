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

#include <memory>

#include <QMap>
#include <QCryptographicHash>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkDiskCache>
#include <QQuickImageProvider>

#include "reactimageloader.h"
#include "reactbridge.h"


class ReactImageLoaderPrivate {
public:
  class ImageProvider : public QQuickImageProvider {
  public:
    ImageProvider(ReactImageLoaderPrivate* priv):QQuickImageProvider(QQuickImageProvider::Image), p(priv) {}
    QImage requestImage(const QString& id,  QSize* size, const QSize& requestedSize) override {
      auto cache = qobject_cast<QNetworkDiskCache*>(p->bridge->networkAccessManager()->cache());
      auto idev = cache->data(p->cacheIds.key(id.toLocal8Bit()));
      if (idev == nullptr) {
        qWarning() << __PRETTY_FUNCTION__ << "Could not obtain cache entry for" << id;
        return QImage();
      }
      idev->deleteLater();
      QImage image = QImage::fromData(idev->readAll());
      if (size != nullptr)
        *size = image.size();
      return image;
    }
    ReactImageLoaderPrivate* p = nullptr;
  };

  ReactImageLoaderPrivate() {
    provider = new ImageProvider(this);
  }
  void markCached(const QUrl& source) {
    cacheIds.insert(source, QCryptographicHash::hash(source.toEncoded(), QCryptographicHash::Sha1).toBase64());
  }
  bool isCached(const QUrl& source) {
    return cacheIds.contains(source);
  }
  QUrl cachedUrl(const QUrl& source) {
    return QUrl("image://react/" + cacheIds.value(source));
  }
  void fetchImage(const QUrl& source, const ReactImageLoader::LoadEventCallback& ec) {
    auto data = std::make_shared<QVariantMap>(QVariantMap{});

    // XXX: images downloading already
    if (isCached(source)) {
      // TODO: need to cycle through events?
      ec(ReactImageLoader::Event_LoadStart, *data);
      ec(ReactImageLoader::Event_LoadSuccess, *data);
      ec(ReactImageLoader::Event_LoadEnd, *data);
      return;
    }

    QNetworkRequest request(source);
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);
    auto reply = bridge->networkAccessManager()->get(request);

    ec(ReactImageLoader::Event_LoadStart, *data);

    QObject::connect(reply, &QNetworkReply::downloadProgress, [=](qint64 bytesReceived, qint64 bytesTotal) {
        data->insert("loaded", bytesReceived);
        data->insert("total", bytesTotal);
        ec(ReactImageLoader::Event_Progress, *data);
      });
    QObject::connect(reply, static_cast<void(QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), [=](QNetworkReply::NetworkError code) {
        data->insert("error", reply->errorString());
        ec(ReactImageLoader::Event_LoadError, *data);
      });
    QObject::connect(reply, &QNetworkReply::finished, [=]() {
        reply->deleteLater();
        if (reply->error() == QNetworkReply::NoError) {
          if (!source.isLocalFile())
            markCached(source);
          ec(ReactImageLoader::Event_LoadSuccess, *data);
        }
        ec(ReactImageLoader::Event_LoadEnd, *data);
      });
  }
  QMap<QUrl, QByteArray> cacheIds;
  ReactBridge* bridge = nullptr;
  ImageProvider* provider = nullptr;
};


void ReactImageLoader::prefetchImage(
  const QString& url,
  const ReactModuleInterface::ListArgumentBlock& resolve,
  const ReactModuleInterface::ListArgumentBlock& reject
) {
  Q_D(ReactImageLoader);
  d->fetchImage(url, [=](ReactImageLoader::Event event, const QVariantMap& data) {
    if (event == Event_LoadEnd) {
      if (data.contains("error"))
        reject(d->bridge, QVariantList{data});
      else
        resolve(d->bridge, QVariantList{true});
    }
  });
}


ReactImageLoader::ReactImageLoader(QObject* parent)
  : QObject(parent)
  , d_ptr(new ReactImageLoaderPrivate)
{
}

ReactImageLoader::~ReactImageLoader()
{
}

void ReactImageLoader::setBridge(ReactBridge* bridge)
{
  Q_D(ReactImageLoader);
  d->bridge = bridge;
  d->bridge->qmlEngine()->addImageProvider("react", d->provider);
}

QString ReactImageLoader::moduleName()
{
  return "RCTImageLoader";
}

QList<ReactModuleMethod*> ReactImageLoader::methodsToExport()
{
  return QList<ReactModuleMethod*>{};
}

QVariantMap ReactImageLoader::constantsToExport()
{
  return QVariantMap{};
}

QUrl ReactImageLoader::provideUriFromSourceUrl(const QUrl& source)
{
  Q_D(ReactImageLoader);
  if (d->isCached(source))
    return d->cachedUrl(source);

  return source;
}

void ReactImageLoader::loadImage(
  const QUrl& source,
  const LoadEventCallback& loadEventCallback
) {
  d_func()->fetchImage(source, loadEventCallback);
}
