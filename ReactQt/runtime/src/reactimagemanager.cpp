
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

#include "reactattachedproperties.h"
#include "reactbridge.h"
#include "reactimageloader.h"
#include "reactimagemanager.h"
#include "reactpropertyhandler.h"
#include "utilities.h"

using namespace utilities;

namespace {
static QMap<ReactImageLoader::Event, QString> eventNames{{ReactImageLoader::Event_LoadStart, "onLoadStart"},
                                                         {ReactImageLoader::Event_Progress, "onProgress"},
                                                         {ReactImageLoader::Event_LoadError, "onError"},
                                                         {ReactImageLoader::Event_LoadSuccess, "onLoad"},
                                                         {ReactImageLoader::Event_LoadEnd, "onLoadEnd"}};
}

class ReactImageManagerPrivate {

public:
    bool isBase64ImageUrl(const QUrl& url) const;
    void setSource(QObject* image, const QUrl& url);
};

ReactImageManager::ReactImageManager(QObject* parent) : ReactViewManager(parent), d_ptr(new ReactImageManagerPrivate) {}

ReactImageManager::~ReactImageManager() {}

ReactViewManager* ReactImageManager::viewManager() {
    return this;
}

QString ReactImageManager::moduleName() {
    return "RCTImageViewManager";
}

QStringList ReactImageManager::customDirectEventTypes() {
    return QStringList{normalizeInputEventName("onLoadStart"),
                       normalizeInputEventName("onProgress"),
                       normalizeInputEventName("onError"),
                       normalizeInputEventName("onLoad"),
                       normalizeInputEventName("onLoadEnd")};
}

void ReactImageManager::manageSource(const QVariantMap& imageSource, QQuickItem* image) {
    Q_D(ReactImageManager);

    auto imageLoader = bridge()->imageLoader();

    QUrl source = imageSource["uri"].toUrl();

    if (d->isBase64ImageUrl(source)) {
        d->setSource(image, source);
        return;
    }

    if (source.isRelative()) {
        source = QUrl::fromLocalFile(QFileInfo(imageSource["uri"].toString()).absoluteFilePath());
    }

    imageLoader->loadImage(source, [=](ReactImageLoader::Event event, const QVariantMap& data) {
        if (event == ReactImageLoader::Event_LoadSuccess) {
            d->setSource(image, source);
        }
        bool eventHandlerSet =
            image->property(QString(QML_PROPERTY_PREFIX + eventNames[event]).toStdString().c_str()).toBool();
        if (eventHandlerSet) {
            notifyJsAboutEvent(tag(image), eventNames[event], data);
        }
    });
}

void ReactImageManager::configureView(QQuickItem* view) const {
    ReactViewManager::configureView(view);
    view->setProperty("imageManager", QVariant::fromValue((QObject*)this));
    view->setEnabled(false);
}

QString ReactImageManager::qmlComponentFile() const {
    return ":/qml/ReactImage.qml";
}

bool ReactImageManagerPrivate::isBase64ImageUrl(const QUrl& url) const {
    return url.toString().startsWith("data:image/png;base64");
}

void ReactImageManagerPrivate::setSource(QObject* image, const QUrl& url) {
    image->setProperty("managedSource", url);
}

#include "reactimagemanager.moc"
