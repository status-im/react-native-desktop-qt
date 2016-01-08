
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

QQuickItem* UbuntuScrollViewManager::view(const QVariantMap& properties) const
{
  return new QQuickItem();
}

