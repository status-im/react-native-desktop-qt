#include <QString>
#include <QVariant>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQmlProperty>

#include <QDebug>

#include "reacttextproperties.h"
#include "reacttextmanager.h"
#include "reactbridge.h"


ReactTextManager::ReactTextManager(QObject* parent)
  : ReactRawTextManager(parent)
{
}

ReactTextManager::~ReactTextManager()
{
}

void ReactTextManager::setBridge(ReactBridge* bridge)
{
  m_bridge = bridge;
}

ReactViewManager* ReactTextManager::viewManager()
{
  return this;
}

QString ReactTextManager::moduleName()
{
  return "RCTTextManager";
}

QStringList ReactTextManager::methodsToExport()
{
  return QStringList{};
}

QVariantMap ReactTextManager::constantsToExport()
{
  return QVariantMap{};
}

QQuickItem* ReactTextManager::view(const QVariantMap& properties) const
{
  QQmlComponent component(m_bridge->qmlEngine());
  component.setData("import QtQuick 2.4\nText{}", QUrl()); // TODO: depends on self text
  if (!component.isReady())
    qCritical() << "Component for RCTTextManager not ready";

  QQuickItem* item = qobject_cast<QQuickItem*>(component.create());
  if (item == nullptr) {
    qCritical() << "Unable to create component for RCTTextManager";
    return nullptr;
  }

  // item->setEnabled(false);

  applyProperties(item, properties);

  return item;
}

void ReactTextManager::applyProperties(QQuickItem* item, const QVariantMap& properties) const
{
  qDebug() << __PRETTY_FUNCTION__ << item << properties;
  if (properties.isEmpty())
    return;

  ReactViewManager::applyProperties(item, properties);

  // These are added to the text style to be used by self(? - TODO) and all
  // descendants
  ReactTextProperties* rtp = ReactTextProperties::get(item);

  for (const QString& key : properties.keys()) {
    if (key == "fontFamily") {
      rtp->setFontFamily(properties.value(key).toString());
    } else if (key == "fontSize") {
      rtp->setFontSize(properties.value(key).toDouble());
    } else if (key == "color") {
      rtp->setColor(QColor(properties.value(key).toUInt()));
    }
  }
}

