
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


class ImagePropertyHandler : public ReactPropertyHandler {
  Q_OBJECT
  Q_PROPERTY(QString backfaceVisibility READ backfaceVisibility WRITE setBackfaceVisibility)
  Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor);
  Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor)
  Q_PROPERTY(double borderWidth READ borderWidth WRITE setBorderWidth)
  Q_PROPERTY(double borderRadius READ borderRadius WRITE setBorderRadius)
  Q_PROPERTY(QString overflow READ overflow WRITE setOverflow)
  Q_PROPERTY(QColor tintColor READ tintColor WRITE setTintColor)
  Q_PROPERTY(double opacity READ opacity WRITE setOpacity)
  Q_PROPERTY(QVariantMap source READ source WRITE setSource)

public:
  ImagePropertyHandler(QObject* object, ReactBridge* bridge)
    : ReactPropertyHandler(object)
    , m_bridge(bridge) {
    }

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

private:
  ReactBridge* m_bridge;
  QMap<ReactImageLoader::Event, bool> m_onLoadEvents;
  bool m_onLayout=false;
};

QColor ImagePropertyHandler::backgroundColor() const
{
  m_object->property("backgroundColor").value<QColor>();
}

void ImagePropertyHandler::setBackgroundColor(const QColor& backgroundColor)
{
  m_object->setProperty("backgroundColor", backgroundColor);
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
  //return new ImagePropertyHandler(object, m_bridge);
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
  QQmlComponent component(m_bridge->qmlEngine());
  component.loadUrl(QUrl::fromLocalFile(":/qml/ReactImage.qml"));
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
