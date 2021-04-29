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

#include <QCryptographicHash>
#include <QMap>
#include <QNetworkDiskCache>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QQuickImageProvider>

#include "bridge.h"
#include "imageloader.h"

class ImageLoaderPrivate {

public:
    QIODevice* cachedImageData(const QUrl& source) {
        auto cache = qobject_cast<QNetworkDiskCache*>(bridge->networkAccessManager()->cache());
        return cache->data(source);
    }

    bool isCached(const QUrl& source) {
        auto cached = cachedImageData(source);
        cached->deleteLater();
        return (cached != nullptr);
    }

    void fetchImage(const QUrl& source, const ImageLoader::LoadEventCallback& ec) {
        auto data = std::make_shared<QVariantMap>(QVariantMap{});

        // XXX: images downloading already
        if (isCached(source)) {
            // TODO: need to cycle through events?
            ec(ImageLoader::Event_LoadStart, *data);
            ec(ImageLoader::Event_LoadSuccess, *data);
            ec(ImageLoader::Event_LoadEnd, *data);
            return;
        }

        QNetworkRequest request(source);
        request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);
        auto reply = bridge->networkAccessManager()->get(request);

        ec(ImageLoader::Event_LoadStart, *data);

        QObject::connect(reply, &QNetworkReply::downloadProgress, [=](qint64 bytesReceived, qint64 bytesTotal) {
            data->insert("loaded", bytesReceived);
            data->insert("total", bytesTotal);
            ec(ImageLoader::Event_Progress, *data);
        });
        QObject::connect(reply,
                         static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error),
                         [=](QNetworkReply::NetworkError code) {
                             data->insert("error", reply->errorString());
                             ec(ImageLoader::Event_LoadError, *data);
                         });
        QObject::connect(reply, &QNetworkReply::finished, [=]() {
            reply->deleteLater();
            if (reply->error() == QNetworkReply::NoError) {
                ec(ImageLoader::Event_LoadSuccess, *data);
            } else {
                qDebug() << "ERROR: " << reply->errorString();
            }
            ec(ImageLoader::Event_LoadEnd, *data);
        });
    }

public:
    Bridge* bridge = nullptr;
};

void ImageLoader::prefetchImage(const QString& url, double success, double error) {
    Q_D(ImageLoader);

    d->fetchImage(url, [=](ImageLoader::Event event, const QVariantMap& data) {
        if (event == ImageLoader::Event_LoadSuccess) {
            d->bridge->invokePromiseCallback(success, QVariantList{});
        }
        if (event == ImageLoader::Event_LoadError) {
            d->bridge->invokePromiseCallback(error, QVariantList{});
        }
    });
}

void ImageLoader::getSize(const QString& url, double success, double error) {
    Q_D(ImageLoader);
    d->fetchImage(url, [=](ImageLoader::Event event, const QVariantMap& data) {
        if (event == ImageLoader::Event_LoadSuccess) {
            auto data = d->cachedImageData(url);
            if (data) {
                QSize size;
                data->deleteLater();
                size = QImage::fromData(data->readAll()).size();
                d->bridge->invokePromiseCallback(
                    success, QVariantList{QVariantMap{{"height", size.height()}, {"width", size.width()}}});
            } else {
                d->bridge->invokePromiseCallback(error, QVariantList{});
            }
        }
        if (event == ImageLoader::Event_LoadError) {
            d->bridge->invokePromiseCallback(error, QVariantList{});
        }
    });
}

ImageLoader::ImageLoader(QObject* parent) : QObject(parent), d_ptr(new ImageLoaderPrivate) {}

ImageLoader::~ImageLoader() {}

void ImageLoader::setBridge(Bridge* bridge) {
    Q_D(ImageLoader);
    d->bridge = bridge;
}

QString ImageLoader::moduleName() {
    return "RCTImageLoader";
}

void ImageLoader::loadImage(const QUrl& source, const LoadEventCallback& loadEventCallback) {
    d_func()->fetchImage(source, loadEventCallback);
}
