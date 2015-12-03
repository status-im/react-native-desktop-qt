#include <QString>
#include <QVariant>
#include <QQuickItem>
#include <QQmlProperty>

#include "reacttextproperties.h"
#include "reactrawtextmanager.h"
#include "reactbridge.h"


ReactRawTextManager::ReactRawTextManager(QObject *parent)
  : ReactViewManager(parent)
{
}

ReactRawTextManager::~ReactRawTextManager()
{
}

void ReactRawTextManager::setBridge(ReactBridge* bridge)
{
  m_bridge = bridge;
}

// TODO: this doesnt seem right
ReactViewManager* ReactRawTextManager::viewManager()
{
  return this;
}

QString ReactRawTextManager::moduleName()
{
  return "RCTRawTextManager";
}

QStringList ReactRawTextManager::methodsToExport()
{
  return QStringList{};
}

QVariantMap ReactRawTextManager::constantsToExport()
{
  return QVariantMap{};
}

QQuickItem* ReactRawTextManager::view(const QVariantMap& properties) const
{
  qDebug() << __PRETTY_FUNCTION__ << "properties" << properties;

  QQmlComponent component(m_bridge->qmlEngine());
  component.setData("import QtQuick 2.4\nText{horizontalAlignment:Text.AlignHCenter;verticalAlignment:Text.AlignVCenter;}", QUrl());
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

void ReactRawTextManager::applyProperties(QQuickItem* item, const QVariantMap& properties) const
{
  qDebug() << __PRETTY_FUNCTION__ << item << properties;

  if (properties.isEmpty())
    return;

  ReactViewManager::applyProperties(item, properties);

  if (properties.contains("text")) {
    item->setProperty("text", properties.value("text"));
  }

  // Grab style from parent text item
  // XXX: we can most likely not apply these sensibly the first time
  // because the item has not been added to the visual hierarchy
  ReactTextProperties *rtp = ReactTextProperties::get(item->parentItem(), false);
  if (rtp == nullptr)
    return; // XXX: prob should travel back up to the root text node

  { QQmlProperty p(item, "font.pointSize");
  p.write(QVariant::fromValue(rtp->fontSize())); }

  { QQmlProperty p(item, "font.family");
    p.write(QVariant::fromValue(rtp->fontFamily())); }

  item->setProperty("color", rtp->color());
}
