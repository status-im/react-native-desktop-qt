#include <QString>
#include <QVariant>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQmlProperty>

#include <QDebug>

#include "reacttextproperties.h"
#include "ubuntutextmanager.h"
#include "reactbridge.h"


UbuntuTextManager::UbuntuTextManager(QObject* parent)
  : UbuntuRawTextManager(parent)
{
}

UbuntuTextManager::~UbuntuTextManager()
{
}

void UbuntuTextManager::setBridge(ReactBridge* bridge)
{
  m_bridge = bridge;
}

UbuntuViewManager* UbuntuTextManager::viewManager()
{
  return this;
}

QString UbuntuTextManager::moduleName()
{
  return "RCTTextManager";
}

QStringList UbuntuTextManager::methodsToExport()
{
  return QStringList{};
}

QVariantMap UbuntuTextManager::constantsToExport()
{
  return QVariantMap{};
}

QQuickItem* UbuntuTextManager::view(const QVariantMap& properties) const
{
  QQmlComponent component(m_bridge->qmlEngine());
  component.setData("import QtQuick 2.4\nItem{}", QUrl()); // TODO: depends on self text
  if (!component.isReady())
    qCritical() << "Component for RCTTextManager not ready";

  QQuickItem* item = qobject_cast<QQuickItem*>(component.create());
  if (item == nullptr) {
    qCritical() << "Unable to create component for RCTTextManager";
    return nullptr;
  }

  //
  item->setEnabled(false);

  applyProperties(item, properties);

  return item;
}


void UbuntuTextManager::applyProperties(QQuickItem* item, const QVariantMap& properties) const
{
  qDebug() << __PRETTY_FUNCTION__ << item << properties;
  if (properties.isEmpty())
    return;

  UbuntuViewManager::applyProperties(item, properties);

  // These are added to the text style to be used by self(? - TODO) and all
  // descendants
  ReactTextProperties* rtp = ReactTextProperties::get(item);

  qDebug() << "SETTING TEXT PROPERTIES" << rtp;
  for (const QString& key : properties.keys()) {
    if (key == "fontFamily") {
      rtp->setFontFamily(properties.value(key).toString());
    } else if (key == "fontSize") {
      rtp->setFontSize(properties.value(key).toDouble());
    } else if (key == "color") {
      rtp->setProperty("color", QColor(properties.value(key).toUInt()));
    }
  }
}

