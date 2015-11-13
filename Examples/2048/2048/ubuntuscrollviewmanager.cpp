
#include <QString>
#include <QVariant>
#include <QQmlComponent>
#include <QQuickItem>

#include <QDebug>

#include "ubuntuscrollviewmanager.h"
#include "reactbridge.h"

UbuntuScrollViewManager::UbuntuScrollViewManager(QObject* parent)
  : UbuntuViewManager(parent)
{
}

UbuntuScrollViewManager::~UbuntuScrollViewManager()
{
}

void UbuntuScrollViewManager::setBridge(ReactBridge* bridge)
{
  m_bridge = bridge;
}

UbuntuViewManager* UbuntuScrollViewManager::viewManager()
{
  return this;
}

QString UbuntuScrollViewManager::moduleName()
{
  return "RCTScrollViewManager";
}

QStringList UbuntuScrollViewManager::methodsToExport()
{
  return QStringList{};
}

QVariantMap UbuntuScrollViewManager::constantsToExport()
{
  return QVariantMap{};
}

QQuickItem* UbuntuScrollViewManager::view(const QVariantMap& properties) const
{
  return new QQuickItem();
}

