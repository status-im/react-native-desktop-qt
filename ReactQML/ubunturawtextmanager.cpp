#include <QString>
#include <QVariant>
#include <QQuickItem>
#include <QQmlProperty>

#include "reacttextproperties.h"
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
  component.setData("import QtQuick 2.4\nText{anchors.centerIn:parent;}", QUrl());
  if (!component.isReady())
    qCritical() << "Component for RCTRawTextManager not ready";

  QQuickItem* item = qobject_cast<QQuickItem*>(component.create());
  if (item == nullptr) {
    qCritical() << "Unable to create component for RCTRawTextManager";
    return nullptr;
  }

  //
  item->setEnabled(false);

  applyProperties(item, properties);

  return item;
}

void UbuntuRawTextManager::applyProperties(QQuickItem* item, const QVariantMap& properties) const
{
  qDebug() << __PRETTY_FUNCTION__ << item << properties;

  if (properties.isEmpty())
    return;

  UbuntuViewManager::applyProperties(item, properties);

  if (properties.contains("text")) {
    item->setProperty("text", properties.value("text"));
  }

  // Grab style from parent text item
  ReactTextProperties *rtp = ReactTextProperties::get(item->parentItem(), false);
  if (rtp == nullptr)
    return; // XXX: prob should travel back up to the root text node

  { QQmlProperty p(item, "font.pointSize");
  p.write(QVariant::fromValue(rtp->fontSize())); }

  { QQmlProperty p(item, "font.family");
    p.write(QVariant::fromValue(rtp->fontFamily())); }

  item->setProperty("color", rtp->color());
}
