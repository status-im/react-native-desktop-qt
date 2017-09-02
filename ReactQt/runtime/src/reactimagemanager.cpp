
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

#include <QString>
#include <QVariant>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQmlProperty>

#include <QDebug>

#include "reactimagemanager.h"
#include "reactattachedproperties.h"
#include "reactbridge.h"
#include "reactevents.h"
#include "reactimageloader.h"
#include "qmlpropertyhandler.h"

int ReactImageManager::m_id = 0;


namespace {
static QMap<ReactImageLoader::Event, QString> eventNames{
  { ReactImageLoader::Event_LoadStart, "onLoadStart" },
  { ReactImageLoader::Event_Progress, "onProgress" },
  { ReactImageLoader::Event_LoadError, "onError" },
  { ReactImageLoader::Event_LoadSuccess, "onLoad" },
  { ReactImageLoader::Event_LoadEnd, "onLoadEnd" }
};
}

ReactImageManager::ReactImageManager(QObject* parent)
  : ReactViewManager(parent)
{
}

ReactImageManager::~ReactImageManager()
{
}

void ReactImageManager::setBridge(ReactBridge* bridge)
{
  m_bridge = bridge;
}

ReactViewManager* ReactImageManager::viewManager()
{
  return this;
}

ReactPropertyHandler* ReactImageManager::propertyHandler(QObject* object)
{
  return new QmlPropertyHandler(object);
}

QString ReactImageManager::moduleName()
{
  return "RCTImageViewManager";
}

QList<ReactModuleMethod*> ReactImageManager::methodsToExport()
{
  return QList<ReactModuleMethod*>{};
}

QVariantMap ReactImageManager::constantsToExport()
{
  return QVariantMap{};
}

QStringList ReactImageManager::customDirectEventTypes()
{
  return QStringList{normalizeInputEventName("onLoadStart"),
                     normalizeInputEventName("onProgress"),
                     normalizeInputEventName("onError"),
                     normalizeInputEventName("onLoad"),
                     normalizeInputEventName("onLoadEnd")};
}

QQuickItem* ReactImageManager::view(const QVariantMap& properties) const
{
  QQuickItem* item = createViewFromFile(":/qml/ReactImage.qml");
  if(item)
  {
    configureView(item);
  }
  return item;
}

void ReactImageManager::manageSource(const QVariantMap& imageSource, QObject* image)
{
  QUrl source = imageSource["uri"].toUrl();
  if (source.isRelative())
  {
    source = QUrl::fromLocalFile(QFileInfo(imageSource["uri"].toString()).absoluteFilePath());
  }

  m_bridge->imageLoader()->loadImage(source, [=](ReactImageLoader::Event event, const QVariantMap& data)
  {
      if (event == ReactImageLoader::Event_LoadSuccess)
      {
        image->setProperty("managedSource", m_bridge->imageLoader()->provideUriFromSourceUrl(source));
      }
      if (image->property(QString(QML_PROPERTY_PREFIX + eventNames[event]).toStdString().c_str()).toBool())
      {
        int reactTag = ReactAttachedProperties::get(qobject_cast<QQuickItem*>(image))->tag();
        m_bridge->enqueueJSCall("RCTEventEmitter", "receiveEvent",
                                QVariantList{reactTag, normalizeInputEventName(eventNames[event]), data});
      }
    });
}

void ReactImageManager::configureView(QQuickItem* view) const
{
  view->setProperty("imageManager", QVariant::fromValue((QObject*)this));
  view->setEnabled(false);
}

#include "reactimagemanager.moc"
