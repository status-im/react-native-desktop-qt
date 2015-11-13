#include <QString>
#include <QVariant>
#include <QQuickItem>

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

  // Set properties
  if (!properties.isEmpty()) {
    Q_FOREACH(const QString& key, properties.keys()) {
      if (key == "text") {
        item->setProperty("text", properties.value(key));
      }

      // XXX: hmm
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
