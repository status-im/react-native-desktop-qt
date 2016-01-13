
#include <QString>
#include <QVariant>
#include <QQmlComponent>
#include <QQuickItem>

#include <QDebug>

#include "ubuntuscrollviewmanager.h"
#include "reactbridge.h"

UbuntuScrollViewManager::UbuntuScrollViewManager(QObject* parent)
  : ReactViewManager(parent)
{
}

UbuntuScrollViewManager::~UbuntuScrollViewManager()
{
}

void UbuntuScrollViewManager::setBridge(ReactBridge* bridge)
{
  m_bridge = bridge;
}

ReactViewManager* UbuntuScrollViewManager::viewManager()
{
  return this;
}

QString UbuntuScrollViewManager::moduleName()
{
  return "RCTScrollViewManager";
}

QList<ReactModuleMethod*> UbuntuScrollViewManager::methodsToExport()
{
  return QList<ReactModuleMethod*>{};
}

QVariantMap UbuntuScrollViewManager::constantsToExport()
{
  return QVariantMap{};
}

namespace {
static const char* component_qml =
"import QtQuick 2.4\n"
"import Ubuntu.Components 1.2\n"
"\n"
"Flickable {\n"
"  id: flikka\n"
"  Scrollbar {\n"
"    flickableItem: flikka\n"
"    "
"  }\n"
"}\n";
}

QQuickItem* UbuntuScrollViewManager::view(const QVariantMap& properties) const
{
  QQmlComponent component(m_bridge->qmlEngine());
  component.setData(component_qml, QUrl());
  if (!component.isReady())
    qCritical() << "Component for UbuntuScrollView not ready";

  QQuickItem* item = qobject_cast<QQuickItem*>(component.create());
  if (item == nullptr) {
    qCritical() << "Unable to create component for UbuntuScrollViewManager";
    return nullptr;
  }

  applyProperties(item, properties);

  return item;
}
