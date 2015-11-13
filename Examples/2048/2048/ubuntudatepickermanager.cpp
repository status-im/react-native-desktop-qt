#include <QString>
#include <QVariant>
#include <QQmlComponent>
#include <QQuickItem>

#include <QDebug>

#include "ubuntuactivitymanager.h"
#include "reactbridge.h"

UbuntuDatePickerManager::UbuntuDatePickerManager(QObject* parent)
  : UbuntuViewManager(parent)
{
}

UbuntuDatePickerManager::~UbuntuDatePickerManager()
{
}

void UbuntuDatePickerManager::setBridge(ReactBridge* bridge)
{
  m_bridge = bridge;
}

UbuntuViewManager* UbuntuDatePickerManager::viewManager()
{
  return this;
}

QString UbuntuDatePickerManager::moduleName()
{
  return "RCTActivityIndicatorViewManager";
}

QStringList UbuntuDatePickerManager::methodsToExport()
{
  return QStringList{};
}

QVariantMap UbuntuDatePickerManager::constantsToExport()
{
  return QVariantMap{ {"running", "bool"} };
}

QQuickItem* UbuntuDatePickerManager::view(const QVariantMap& properties) const
{
  QQmlComponent component(m_bridge->qmlEngine(), "/usr/lib/x86_64-linux-gnu/qt5/qml/Ubuntu/Components/ActivityIndicator.qml");
  // XXX: nothing should begin until components have successfully been ctored and inspected)
  while (!component.isReady())
    QCoreApplication::processEvents();
  QObject* o = component.create();
  QQuickItem* item = qobject_cast<QQuickItem*>(o);
  item->setParentItem(m_bridge->visualParent());
  return item;
}

