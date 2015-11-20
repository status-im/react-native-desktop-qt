#include <QString>
#include <QVariant>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQmlProperty>

#include <QDebug>

#include "ubuntutextmanager.h"
#include "reactbridge.h"


UbuntuTextManager::UbuntuTextManager(QObject* parent)
  : UbuntuViewManager(parent)
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
  // QQmlComponent component(m_bridge->qmlEngine(),
  //                         "/usr/lib/x86_64-linux-gnu/qt5/qml/Ubuntu/Components/TextArea.qml",
  //                         QQmlComponent::PreferSynchronous);
  QQmlComponent component(m_bridge->qmlEngine());
  component.setData("import QtQuick 2.4\nText{}", QUrl());
  if (!component.isReady())
    qCritical() << "Component for RCTTextManager not ready";

  QQuickItem* item = qobject_cast<QQuickItem*>(component.create());
  if (item == nullptr) {
    qCritical() << "Unable to create component for RCTTextManager";
    return nullptr;
  }

  applyProperties(item, properties);

  return item;
}


void UbuntuTextManager::applyProperties(QQuickItem* item, const QVariantMap& properties) const
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

void UbuntuTextManager::updateLayout(QQuickItem* item, const QVariantMap& properties) const
{
}
