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

#include "reactbridge.h"
#include "reactimageloader.h"

class ReactImageLoaderPrivate {

public:
    QIODevice* cachedData(const QUrl& source) {
        auto cache = qobject_cast<QNetworkDiskCache*>(bridge->networkAccessManager()->cache());
        return cache->data(source);
    }

    bool isCached(const QUrl& source) {
        auto cached = cachedData(source);
        cached->deleteLater();
        return (cached != nullptr);
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
        QObject::connect(reply,
                         static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error),
                         [=](QNetworkReply::NetworkError code) {
                             data->insert("error", reply->errorString());
                             ec(ReactImageLoader::Event_LoadError, *data);
                         });
        QObject::connect(reply, &QNetworkReply::finished, [=]() {
            reply->deleteLater();
            if (reply->error() == QNetworkReply::NoError) {
                ec(ReactImageLoader::Event_LoadSuccess, *data);
            } else {
                qDebug() << "ERROR: " << reply->errorString();
            }
            ec(ReactImageLoader::Event_LoadEnd, *data);
        });
    }

    ReactBridge* bridge = nullptr;
};

void ReactImageLoader::prefetchImage(const QString& url, double success, double error) {
    Q_D(ReactImageLoader);

    d->fetchImage(url, [=](ReactImageLoader::Event event, const QVariantMap& data) {
        if (event == ReactImageLoader::Event_LoadSuccess) {
            d->bridge->invokePromiseCallback(success, QVariantList{});
        }
        if (event == ReactImageLoader::Event_LoadError) {
            d->bridge->invokePromiseCallback(error, QVariantList{});
        }
    });
}

void ReactImageLoader::getSize(const QString& url, double success, double error) {
    Q_D(ReactImageLoader);
    d->fetchImage(url, [=](ReactImageLoader::Event event, const QVariantMap& data) {
        if (event == ReactImageLoader::Event_LoadSuccess) {
            if (d->isCached(url)) {
                QSize size;
                auto data = d->cachedData(url);
                data->deleteLater();
                size = QImage::fromData(data->readAll()).size();
                d->bridge->invokePromiseCallback(
                    success, QVariantList{QVariantMap{{"height", size.height()}, {"width", size.width()}}});
            } else {
                d->bridge->invokePromiseCallback(error, QVariantList{});
            }
        }
        if (event == ReactImageLoader::Event_LoadError) {
            d->bridge->invokePromiseCallback(error, QVariantList{});
        }
    });
}

ReactImageLoader::ReactImageLoader(QObject* parent) : QObject(parent), d_ptr(new ReactImageLoaderPrivate) {}

ReactImageLoader::~ReactImageLoader() {}

void ReactImageLoader::setBridge(ReactBridge* bridge) {
    Q_D(ReactImageLoader);
    d->bridge = bridge;
}

QString ReactImageLoader::moduleName() {
    return "RCTImageLoader";
}

QList<ReactModuleMethod*> ReactImageLoader::methodsToExport() {
    return QList<ReactModuleMethod*>{};
}

QVariantMap ReactImageLoader::constantsToExport() {
    return QVariantMap{};
}

void ReactImageLoader::loadImage(const QUrl& source, const LoadEventCallback& loadEventCallback) {
    d_func()->fetchImage(source, loadEventCallback);
}
