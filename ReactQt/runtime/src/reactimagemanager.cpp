
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
#include "reactpropertyhandler.h"
#include "reactattachedproperties.h"
#include "reactbridge.h"
#include "reactevents.h"
#include "reactimageloader.h"

int ReactImageManager::m_id = 0;


namespace {
static QMap<ReactImageLoader::Event, QString> eventNames{
  { ReactImageLoader::Event_LoadStart, "onLoadStart" },
  { ReactImageLoader::Event_Progress, "onProgress" },
  { ReactImageLoader::Event_Error, "onError" },
  { ReactImageLoader::Event_Load, "onLoad" },
  { ReactImageLoader::Event_LoadEnd, "onLoadEnd" }
};
}


class ImagePropertyHandler : public ReactPropertyHandler {
  Q_OBJECT
  Q_PROPERTY(QString resizeMode READ resizeMode WRITE setResizeMode)
  Q_PROPERTY(QString backfaceVisibility READ backfaceVisibility WRITE setBackfaceVisibility)
  Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor);
  Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor)
  Q_PROPERTY(double borderWidth READ borderWidth WRITE setBorderWidth)
  Q_PROPERTY(double borderRadius READ borderRadius WRITE setBorderRadius)
  Q_PROPERTY(QString overflow READ overflow WRITE setOverflow)
  Q_PROPERTY(QColor tintColor READ tintColor WRITE setTintColor)
  Q_PROPERTY(double opacity READ opacity WRITE setOpacity)
  Q_PROPERTY(QVariantMap source READ source WRITE setSource)
  Q_PROPERTY(bool onLoadStart READ onLoadStart WRITE setOnLoadStart)
  Q_PROPERTY(bool onProgress READ onProgress WRITE setOnProgress)
  Q_PROPERTY(bool onError READ onError WRITE setOnError)
  Q_PROPERTY(bool onLoad READ onLoad WRITE setOnLoad)
  Q_PROPERTY(bool onLoadEnd READ onLoadEnd WRITE setOnLoadEnd)

public:
  enum FillMode { Stretch, PreserveAspectFit, PreserveAspectCrop, Tile, TileVertically, TileHorizontally, Pad };
  ImagePropertyHandler(QObject* object, ReactBridge* bridge)
    : ReactPropertyHandler(object)
    , m_bridge(bridge) {
    }
  QString resizeMode() const;
  void setResizeMode(const QString& resizeMode);
  QString backfaceVisibility() const;
  void setBackfaceVisibility(const QString& backfaceVisibility);
  QColor backgroundColor() const;
  void setBackgroundColor(const QColor& backgroundColor);
  QColor tintColor() const;
  void setTintColor(const QColor& tintColor);
  double opacity() const;
  void setOpacity(double opacity);
  double borderWidth() const;
  void setBorderWidth(double borderWidth);
  double borderRadius() const;
  void setBorderRadius(double borderRadius);
  QString overflow() const;
  void setOverflow(const QString& overdlow);
  QColor borderColor() const;
  void setBorderColor(const QColor& borderColor);
  QVariantMap source() const;
  void setSource(const QVariantMap& source);
  bool onLoadStart() const;
  void setOnLoadStart(bool onLoadStart);
  bool onProgress() const;
  void setOnProgress(bool onProgress);
  bool onError() const;
  void setOnError(bool onError);
  bool onLoad() const;
  void setOnLoad(bool onLoad);
  bool onLoadEnd() const;
  void setOnLoadEnd(bool onLoadEnd);
private:
  ReactBridge* m_bridge;
  QMap<ReactImageLoader::Event, bool> m_events;
};

QColor ImagePropertyHandler::backgroundColor() const
{
  m_object->property("backgroundColor").value<QColor>();
}

void ImagePropertyHandler::setBackgroundColor(const QColor& backgroundColor)
{
  m_object->setProperty("backgroundColor", backgroundColor);
}

QString ImagePropertyHandler::resizeMode() const
{
  int resizeMode = m_object->property("fillMode").toInt();
  switch (resizeMode) {
    case Stretch: return "stretch";
   case PreserveAspectFit: return "contain";
    case PreserveAspectCrop: return "cover";
    default: return "";
  }
  return "";
}

void ImagePropertyHandler::setResizeMode(const QString& resizeMode)
{
  if (resizeMode == "stretch") {
    m_object->setProperty("resizeMode", QVariant::fromValue(int(Stretch)));
  } else if (resizeMode == "contain") {
    m_object->setProperty("resizeMode", QVariant::fromValue(int(PreserveAspectFit)));
  } else if (resizeMode == "cover") {
    m_object->setProperty("resizeMode", QVariant::fromValue(int(PreserveAspectCrop)));
  }
}

QString ImagePropertyHandler::backfaceVisibility() const
{
  return QString();
}

void ImagePropertyHandler::setBackfaceVisibility(const QString& backfaceVisibility)
{
}

QVariantMap ImagePropertyHandler::source() const
{
  return QVariantMap{};
}

void ImagePropertyHandler::setSource(const QVariantMap& imageSource)
{
  QUrl source = imageSource["uri"].toUrl();
  if (source.isRelative())
    source = QUrl::fromLocalFile(QFileInfo(imageSource["uri"].toString()).absoluteFilePath());

  m_bridge->imageLoader()->loadImage(source, [=](ReactImageLoader::Event event, const QVariantMap& data) {
      if (event == ReactImageLoader::Event_Load) {
        m_object->setProperty("source", m_bridge->imageLoader()->provideUriFromSourceUrl(source));
      }
      if (m_events[event]) {
        int reactTag = ReactAttachedProperties::get(qobject_cast<QQuickItem*>(m_object))->tag();
        m_bridge->enqueueJSCall("RCTEventEmitter", "receiveEvent",
                                QVariantList{reactTag, normalizeInputEventName(eventNames[event]), data});
      }
    });
}

QColor ImagePropertyHandler::tintColor() const
{
  return m_object->property("tintColor").value<QColor>();
}

void ImagePropertyHandler::setTintColor(const QColor& tintColor)
{
  m_object->setProperty("tintColor", tintColor);
}

double ImagePropertyHandler::opacity() const
{
  return m_object->property("opacity").value<double>();
}

void ImagePropertyHandler::setOpacity(double opacity)
{
  m_object->setProperty("opacity", opacity);
}

double ImagePropertyHandler::borderRadius() const
{
  return m_object->property("borderRadius").value<double>();
}

void ImagePropertyHandler::setBorderRadius(double borderRadius)
{
  m_object->setProperty("borderRadius", borderRadius);
}

QString ImagePropertyHandler::overflow() const
{
  return QString();
}

void ImagePropertyHandler::setOverflow(const QString& overflow)
{
  const bool clip = overflow == "hidden";
  m_object->setProperty("clip", clip);
}
double ImagePropertyHandler::borderWidth() const
{
  return m_object->property("borderWidth").value<double>();
}

void ImagePropertyHandler::setBorderWidth(double borderWidth)
{
  QQmlProperty(m_object, "borderWidth").write(borderWidth);
}

QColor ImagePropertyHandler::borderColor() const
{
  return m_object->property("borderColor").value<QColor>();
}

void ImagePropertyHandler::setBorderColor(const QColor& borderColor)
{
  QQmlProperty(m_object, "borderColor").write(borderColor);
}

bool ImagePropertyHandler::onLoadStart() const
{
  return m_events[ReactImageLoader::Event_LoadStart];
}

void ImagePropertyHandler::setOnLoadStart(bool onLoadStart)
{
  m_events[ReactImageLoader::Event_LoadStart] = onLoadStart;
}

bool ImagePropertyHandler::onProgress() const
{
  return m_events[ReactImageLoader::Event_Progress];
}

void ImagePropertyHandler::setOnProgress(bool onProgress)
{
  m_events[ReactImageLoader::Event_Progress] = onProgress;
}

bool ImagePropertyHandler::onError() const
{
  return m_events[ReactImageLoader::Event_Error];
}

void ImagePropertyHandler::setOnError(bool onError)
{
  m_events[ReactImageLoader::Event_Error] = onError;
}

bool ImagePropertyHandler::onLoad() const
{
  return m_events[ReactImageLoader::Event_Load];
}

void ImagePropertyHandler::setOnLoad(bool onLoad)
{
  m_events[ReactImageLoader::Event_Load] = onLoad;
}

bool ImagePropertyHandler::onLoadEnd() const
{
  return m_events[ReactImageLoader::Event_LoadEnd];
}

void ImagePropertyHandler::setOnLoadEnd(bool onLoadEnd)
{
  m_events[ReactImageLoader::Event_LoadEnd] = onLoadEnd;
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
  return new ImagePropertyHandler(object, m_bridge);
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

namespace {
static const char* component_qml = R"COMPONENT(
import QtQuick 2.4
import QtGraphicalEffects 1.0
import React 0.1 as React

React.Item {
  id: imageRect%1
  backgroundColor: 'transparent'
  property alias source: image%1.source
  property alias resizeMode: image%1.fillMode
  property alias tintColor: colorOverlay%1.color
  onTintColorChanged: {
    image%1.visible = false
    colorOverlay%1.visible = true
  }
  Image {
    id: image%1
    visible: true
    anchors.fill: parent
    layer.enabled: imageRect%1.borderRadius > 0
    layer.effect: OpacityMask {
      maskSource: Rectangle {
        width: image%1.width
        height: image%1.height
        radius: imageRect%1.borderRadius
      }
    }
  }
  ColorOverlay {
    visible: false
    anchors.fill: image%1
    id: colorOverlay%1
    source: image%1
  }
}
)COMPONENT";
}

QQuickItem* ReactImageManager::view(const QVariantMap& properties) const
{
  QString componentString = QString(component_qml).arg(m_id++);

  QQmlComponent component(m_bridge->qmlEngine());
  component.setData(componentString.toLocal8Bit(), QUrl());
  if (!component.isReady())
    qCritical() << "Component for RCTImageViewManager not ready" << component.errors();

  QQuickItem* item = qobject_cast<QQuickItem*>(component.create());
  if (item == nullptr) {
    qCritical() << "Unable to create component for RCTImageViewManager";
    return nullptr;
  }

  configureView(item);

  return item;
}

void ReactImageManager::configureView(QQuickItem* view) const
{
  view->setEnabled(false);
}

#include "reactimagemanager.moc"
