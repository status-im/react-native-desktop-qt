
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

#include <QQmlComponent>
#include <QQmlProperty>
#include <QQuickItem>
#include <QString>
#include <QVariant>

#include <QDebug>

#include "attachedproperties.h"
#include "bridge.h"
#include "imageloader.h"
#include "imagemanager.h"
#include "propertyhandler.h"
#include "utilities.h"

using namespace utilities;

namespace {
static QMap<ImageLoader::Event, QString> eventNames{{ImageLoader::Event_LoadStart, "onLoadStart"},
                                                    {ImageLoader::Event_Progress, "onProgress"},
                                                    {ImageLoader::Event_LoadError, "onError"},
                                                    {ImageLoader::Event_LoadSuccess, "onLoad"},
                                                    {ImageLoader::Event_LoadEnd, "onLoadEnd"}};
const QString URI_KEY = QStringLiteral("uri");
const QString FILE_SCHEME = QStringLiteral("file://");
} // namespace

class ImageManagerPrivate {

public:
    bool isBase64ImageUrl(const QUrl& url) const;
    void setSource(QObject* image, const QUrl& url);
    bool isStoredInResources(const QString& fileName);
};

ImageManager::ImageManager(QObject* parent) : ViewManager(parent), d_ptr(new ImageManagerPrivate) {}

ImageManager::~ImageManager() {}

ViewManager* ImageManager::viewManager() {
    return this;
}

QString ImageManager::moduleName() {
    return "RCTImageViewManager";
}

QStringList ImageManager::customDirectEventTypes() {
    return QStringList{normalizeInputEventName("onLoadStart"),
                       normalizeInputEventName("onProgress"),
                       normalizeInputEventName("onError"),
                       normalizeInputEventName("onLoad"),
                       normalizeInputEventName("onLoadEnd")};
}

void ImageManager::manageSource(const QVariantMap& imageSource, QQuickItem* image) {
    Q_D(ImageManager);

    auto imageLoader = bridge()->imageLoader();

    QUrl imageSourceUrl = imageSource[URI_KEY].toUrl();
    QString imageSourceStr = imageSource[URI_KEY].toString();

    if (d->isBase64ImageUrl(imageSourceUrl)) {
        d->setSource(image, imageSourceUrl);
        return;
    }

    image->setProperty("isSVG", imageSourceUrl.toString(QUrl::RemoveQuery).endsWith("svg"));

    if (imageSourceUrl.scheme() == "file" && imageSource[URI_KEY].toString().startsWith(FILE_SCHEME)) {
        imageSourceUrl = QUrl(imageSourceStr.replace(FILE_SCHEME, ""));
    }
    if (d->isStoredInResources(imageSourceStr)) {
        imageSourceUrl = QUrl(QString("qrc:/") + imageSourceStr);
    } else if (imageSourceUrl.isRelative()) {
        imageSourceUrl = QUrl::fromLocalFile(QFileInfo(imageSourceUrl.toString()).absoluteFilePath());
    }

    imageLoader->loadImage(imageSourceUrl, [=](ImageLoader::Event event, const QVariantMap& data) {
        if (event == ImageLoader::Event_LoadSuccess) {
            d->setSource(image, imageSourceUrl);
        }
        bool eventHandlerSet =
            image->property(QString(QML_PROPERTY_PREFIX + eventNames[event]).toStdString().c_str()).toBool();
        if (eventHandlerSet) {
            notifyJsAboutEvent(tag(image), eventNames[event], data);
        }
    });
}

void ImageManager::configureView(QQuickItem* view) const {
    ViewManager::configureView(view);
    view->setProperty("imageManager", QVariant::fromValue((QObject*)this));
    view->setEnabled(false);
}

QString ImageManager::qmlComponentFile(const QVariantMap& properties) const {
    return "qrc:/qml/ReactImage.qml";
}

bool ImageManagerPrivate::isBase64ImageUrl(const QUrl& url) const {
    return url.toString().startsWith("data:image/png;base64");
}

void ImageManagerPrivate::setSource(QObject* image, const QUrl& url) {
    image->setProperty("managedSource", url);
}

bool ImageManagerPrivate::isStoredInResources(const QString& fileName) {

    QFile file(QString(":/") + fileName);
    return file.exists();
}

#include "imagemanager.moc"
