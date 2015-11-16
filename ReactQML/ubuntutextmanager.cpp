#include <QString>
#include <QVariant>
#include <QQmlComponent>
#include <QQuickItem>

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
  QQmlComponent component(m_bridge->qmlEngine(),
                          "/usr/lib/x86_64-linux-gnu/qt5/qml/Ubuntu/Components/TextArea.qml",
                          QQmlComponent::PreferSynchronous);
  // XXX: nothing should begin until components have successfully been ctored and inspected)
  while (!component.isReady()) {
    QCoreApplication::processEvents();
  }
  QObject* o = component.create();
  QQuickItem* item = qobject_cast<QQuickItem*>(o);
  return item;
}

