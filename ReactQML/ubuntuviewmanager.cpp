
#include <QString>
#include <QVariant>
#include <QQuickItem>

#include "ubuntuviewmanager.h"
#include "reactbridge.h"

UbuntuViewManager::UbuntuViewManager(QObject *parent)
  : QObject(parent)
{
}

UbuntuViewManager::~UbuntuViewManager()
{
}

void UbuntuViewManager::setBridge(ReactBridge* bridge)
{
  m_bridge = bridge;
}

// TODO: this doesnt seem right
UbuntuViewManager* UbuntuViewManager::viewManager()
{
  return this;
}

QString UbuntuViewManager::moduleName()
{
  return "RCTViewManager";
}

QStringList UbuntuViewManager::methodsToExport()
{
  return QStringList{};
}

QVariantMap UbuntuViewManager::constantsToExport()
{
  return QVariantMap{};
}

QQuickItem* UbuntuViewManager::view(const QVariantMap& properties) const
{
  qDebug() << __PRETTY_FUNCTION__ << "properties" << properties;

  QQmlComponent component(m_bridge->qmlEngine());
  component.setData("import QtQuick 2.4\nRectangle{}", QUrl());
  if (!component.isReady())
    qCritical() << "Rectangle not ready!";

  QQuickItem* item = qobject_cast<QQuickItem*>(component.create());
  if (item == nullptr) {
    qCritical() << "Unable to construct Rectangle";
    return nullptr;
  }

  // Set properties
  // XXX: hmm, translation rules, etc
  if (!properties.isEmpty()) {
    Q_FOREACH(const QString& key, properties.keys()) {
      if (key == "position") {
        if (properties.value(key) == "absolute") {
          item->setProperty("anchors.fill", "parent");
        }
      }
      if (key == "backgroundColor") {
        item->setProperty("colour", QColor(properties.value(key).toUInt()));
      } else if (key == "borderRadius") {
        item->setProperty("radius", properties.value(key));
      } else if (key == "height") {
        item->setProperty("height", properties.value(key));
      } else if (key == "width") {
        item->setProperty("width", properties.value(key));
      } else if (key == "margin") {
        // TODO; this is valid?
        item->setProperty("anchors.margins", properties.value(key));
      }
    }
  }

  return item;
}

void UbuntuViewManager::applyProperties(QQuickItem* item, const QVariantMap& properties) const {
}
