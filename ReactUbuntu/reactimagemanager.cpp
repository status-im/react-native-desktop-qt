#include <QString>
#include <QVariant>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQmlProperty>

#include <QDebug>

#include "reactimagemanager.h"
#include "reactpropertyhandler.h"
#include "reactbridge.h"


class ImagePropertyHandler : public ReactPropertyHandler {
  Q_OBJECT
  Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor);
  Q_PROPERTY(QString resizeMode READ resizeMode WRITE setResizeMode)
public:
  enum FillMode { Stretch, PreserveAspectFit, PreserveAspectCrop, Tile, TileVertically, TileHorizontally, Pad };
  ImagePropertyHandler(QObject* object)
    : ReactPropertyHandler(object) {
    }
  QColor backgroundColor() const;
  void setBackgroundColor(const QColor& backgroundColor);
  QString resizeMode() const;
  void setResizeMode(const QString& resizeMode);
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
    m_object->setProperty("fillMode", QVariant::fromValue(int(Stretch)));
  } else if (resizeMode == "contain") {
    m_object->setProperty("fillMode", QVariant::fromValue(int(PreserveAspectFit)));
  } else if (resizeMode == "cover") {
    m_object->setProperty("fillMode", QVariant::fromValue(int(PreserveAspectCrop)));
  }
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
static const char* component_qml =
"import QtQuick 2.4\n"
"\n"
"Image {\n"
"  property color backgroundColor: \"transparent\"\n"
"  Rectangle {\n"
"    color: backgroundColor\n"
"    anchors.fill: parent\n"
"  }\n"
"}\n";
}

QQuickItem* ReactImageManager::view(const QVariantMap& properties) const
{
  QString componentString = QString(component_qml);

  QQmlComponent component(m_bridge->qmlEngine());
  component.setData(componentString.toLocal8Bit(), QUrl());
  if (!component.isReady())
    qCritical() << "Component for RCTImageViewManager not ready";

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
