#include <QString>
#include <QVariant>
#include <QQuickItem>
#include <QQmlProperty>

#include "ubunturawtextmanager.h"
#include "reactbridge.h"

UbuntuRawTextManager::UbuntuRawTextManager(QObject *parent)
  : UbuntuViewManager(parent)
{
}

UbuntuRawTextManager::~UbuntuRawTextManager()
{
}

void UbuntuRawTextManager::setBridge(ReactBridge* bridge)
{
  m_bridge = bridge;
}

// TODO: this doesnt seem right
UbuntuViewManager* UbuntuRawTextManager::viewManager()
{
  return this;
}

QString UbuntuRawTextManager::moduleName()
{
  return "RCTRawTextManager";
}

QStringList UbuntuRawTextManager::methodsToExport()
{
  return QStringList{};
}

QVariantMap UbuntuRawTextManager::constantsToExport()
{
  return QVariantMap{};
}

QQuickItem* UbuntuRawTextManager::view(const QVariantMap& properties) const
{
  qDebug() << __PRETTY_FUNCTION__ << "properties" << properties;

  QQmlComponent component(m_bridge->qmlEngine());
  component.setData("import QtQuick 2.4\nText{}", QUrl());
  if (!component.isReady())
    qCritical() << "Component for RCTRawTextManager not ready";

  QQuickItem* item = qobject_cast<QQuickItem*>(component.create());
  if (item == nullptr) {
    qCritical() << "Unable to create component for RCTRawTextManager";
    return nullptr;
  }

  applyProperties(item, properties);

  return item;
}

void UbuntuRawTextManager::applyProperties(QQuickItem* item, const QVariantMap& properties) const
{
  qDebug() << __PRETTY_FUNCTION__ << item << properties;

  if (properties.isEmpty())
    return;

  UbuntuViewManager::applyProperties(item, properties);

  Q_FOREACH(const QString& key, properties.keys()) {
    if (key == "text") {
      item->setProperty("text", properties.value(key));
    } else if (key == "fontFamily") {
      qDebug() << "setting font size";
      QQmlProperty p(item, "font.family");
      p.write(QVariant::fromValue(properties.value(key).toString()));
    } else if (key == "fontSize") {
      qDebug() << "setting font size";
      QQmlProperty p(item, "font.pointSize");
      p.write(QVariant::fromValue(100.0/*properties.value(key).toDouble()*/));
    } else if (key == "color") {
      item->setProperty("color", QColor(properties.value(key).toUInt()));
    }
    // TODO:
    // weight
    // font scaling
  }
}
