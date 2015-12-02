
#include <QString>
#include <QVariant>
#include <QQmlComponent>
#include <QQuickItem>

#include <QDebug>

#include "ubuntucomponentmodule.h"
#include "reactbridge.h"


UbuntuComponentModule::UbuntuComponentModule
(
 const QString& name,
 const QString& version,
 const QString& path
)
  : ReactViewManager(0)
  , m_name(name)
  , m_version(version)
  , m_path(path)
{
}

UbuntuComponentModule::~UbuntuComponentModule()
{
}

ReactViewManager* UbuntuComponentModule::viewManager()
{
  return this;
}

QString UbuntuComponentModule::moduleName()
{
  return "Ubuntu" + m_name + "Manager";
}

QStringList UbuntuComponentModule::methodsToExport()
{
  return QStringList{};
}

QVariantMap UbuntuComponentModule::constantsToExport()
{
  return QVariantMap{};
}

QQuickItem* UbuntuComponentModule::view(const QVariantMap& properties) const
{
  // Should probably build a string with import statement instead of doing this
  QQmlComponent component(m_bridge->qmlEngine(), m_path, QQmlComponent::PreferSynchronous);
  if (!component.isReady())
    qCritical() << m_name << "was not ready!";

  QQuickItem* item = qobject_cast<QQuickItem*>(component.create());
  if (item == nullptr) {
    qCritical() << "Unable to construct Rectangle";
    return nullptr;
  }

  applyProperties(item, properties);

  return item;
}

void UbuntuComponentModule::applyProperties(QQuickItem* item, const QVariantMap& properties) const
{
  qDebug() << __PRETTY_FUNCTION__ << item << properties;
  if (properties.isEmpty())
    return;

  ReactViewManager::applyProperties(item, properties);
}
