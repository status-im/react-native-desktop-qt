#include <QString>
#include <QVariant>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQmlProperty>

#include <QDebug>

#include "reactimagemanager.h"
#include "reactpropertyhandler.h"
#include "reactbridge.h"
#include "reactevents.h"
#include "reactimageloader.h"

int ReactImageManager::m_id = 0;

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
  Q_PROPERTY(QUrl source READ source WRITE setSource)

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
  QUrl source() const;
  void setSource(const QUrl& source);
private:
  ReactBridge* m_bridge;
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

QUrl ImagePropertyHandler::source() const
{
  return QUrl();
}

void ImagePropertyHandler::setSource(const QUrl& source)
{
  m_object->setProperty("source", m_bridge->imageLoader()->provideUriFromSourceUrl(source));
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
  AnimatedImage {
    id: image%1
    visible: true
    anchors.fill: parent
    playing: true
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


void ReactImageManager::statusChanged()
{
  qDebug() << __PRETTY_FUNCTION__;
}

void ReactImageManager::configureView(QQuickItem* view) const
{
  view->setEnabled(false);
  connect(view, SIGNAL(statusChanged(QQuickImageBase::Status)), SLOT(statusChanged()));
}

#include "reactimagemanager.moc"
