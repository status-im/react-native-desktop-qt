#include <QString>
#include <QVariant>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQmlProperty>

#include <QDebug>

#include "reactimagemanager.h"
#include "reactpropertyhandler.h"
#include "reactbridge.h"

int ReactImageManager::m_id = 0;

class ImagePropertyHandler : public ReactPropertyHandler {
  Q_OBJECT
  Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor);
  Q_PROPERTY(QString resizeMode READ resizeMode WRITE setResizeMode)
  Q_PROPERTY(QUrl source READ source WRITE setSource)
  Q_PROPERTY(QColor tintColor READ tintColor WRITE setTintColor)
  Q_PROPERTY(double opacity READ opacity WRITE setOpacity)
  Q_PROPERTY(double borderRadius READ borderRadius WRITE setBorderRadius)
  Q_PROPERTY(double borderWidth READ borderWidth WRITE setBorderWidth)
  Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor)
public:
  enum FillMode { Stretch, PreserveAspectFit, PreserveAspectCrop, Tile, TileVertically, TileHorizontally, Pad };
  ImagePropertyHandler(QObject* object)
    : ReactPropertyHandler(object) {
    } QColor backgroundColor() const;
  void setBackgroundColor(const QColor& backgroundColor);
  QString resizeMode() const;
  void setResizeMode(const QString& resizeMode);
  QUrl source() const;
  void setSource(const QUrl& source);
  QColor tintColor() const;
  void setTintColor(const QColor& tintColor);
  double opacity() const;
  void setOpacity(double opacity);
  double borderRadius();
  void setBorderRadius(double borderRadius);
  double borderWidth();
  void setBorderWidth(double borderWidth);
  QColor borderColor();
  void setBorderColor(const QColor& borderColor);
};

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

void ImagePropertyHandler::setSource(const QUrl& source)
{
  m_object->setProperty("source", source);
}

void ImagePropertyHandler::setTintColor(const QColor& tintColor)
{
  m_object->setProperty("tintColor", tintColor);
}

void ImagePropertyHandler::setOpacity(double opacity)
{
  m_object->setProperty("opacity", opacity);
}

void ImagePropertyHandler::setBorderRadius(double borderRadius)
{
  m_object->setProperty("borderRadius", borderRadius);
}

void ImagePropertyHandler::setBorderWidth(double borderWidth)
{
  QQmlProperty(m_object, "border.width").write(borderWidth);
}

void ImagePropertyHandler::setBorderColor(const QColor& borderColor)
{
  QQmlProperty(m_object, "border.color").write(borderColor);
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
  return new ImagePropertyHandler(object);
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

Rectangle {
  id: imageRect%1
  color: 'transparent'
  property alias backgroundColor: imageRect%1.color
  property alias source: image%1.source
  property alias resizeMode: image%1.fillMode
  property alias tintColor: colorOverlay%1.color
  property real borderRadius: 0
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
  connect(view, SIGNAL(statusChanged(QQuickImageBase::Status)), SLOT(statusChanged()));
}

#include "reactimagemanager.moc"
