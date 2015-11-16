
#include <QString>
#include <QVariant>
#include <QQmlComponent>
#include <QQuickItem>

#include <QDebug>

#include "ubuntuactivitymanager.h"
#include "reactbridge.h"

UbuntuActivityManager::UbuntuActivityManager(QObject* parent)
  : UbuntuViewManager(parent)
{
}

UbuntuActivityManager::~UbuntuActivityManager()
{
}

void UbuntuActivityManager::setBridge(ReactBridge* bridge)
{
  m_bridge = bridge;
}

UbuntuViewManager* UbuntuActivityManager::viewManager()
{
  return this;
}

QString UbuntuActivityManager::moduleName()
{
  return "RCTActivityIndicatorViewManager";
}

QStringList UbuntuActivityManager::methodsToExport()
{
  return QStringList{};
}

QVariantMap UbuntuActivityManager::constantsToExport()
{
  return QVariantMap{};
}

QQuickItem* UbuntuActivityManager::view(const QVariantMap& properties) const
{
  QQmlComponent component(m_bridge->qmlEngine(),
                          "/usr/lib/x86_64-linux-gnu/qt5/qml/Ubuntu/Components/ActivityIndicator.qml",
                          QQmlComponent::PreferSynchronous);
  // XXX: nothing should begin until components have successfully been ctored and inspected)
  // Needs to be wrapped up in a module loader that handles qml modules
  if (!component.isReady())
    qCritical() << "ActivityIndicator not ready!";

  QObject* o = component.create();
  QQuickItem* item = qobject_cast<QQuickItem*>(o);
  return item;
}

