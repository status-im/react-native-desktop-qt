#include <QString>
#include <QVariant>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQmlProperty>

#include <QDebug>

#include "reactimagemanager.h"
#include "reactbridge.h"


ReactImageManager::ReactImageManager(QObject* parent)
  : ReactViewManager(parent)
{
}

ReactImageManager::~ReactImageManager()
{
}

void ReactImageManager::setBridge(ReactBridge* bridge)
{
  m_bridge = bridge;
}

ReactViewManager* ReactImageManager::viewManager()
{
  return this;
}

QString ReactImageManager::moduleName()
{
  return "RCTImageViewManager";
}

QList<ReactModuleMethod*> ReactImageManager::methodsToExport()
{
  return QList<ReactModuleMethod*>{};
}

QVariantMap ReactImageManager::constantsToExport()
{
  return QVariantMap{};
}

QQuickItem* ReactImageManager::view(const QVariantMap& properties) const
{
  QQmlComponent component(m_bridge->qmlEngine());
  component.setData("import QtQuick 2.4\nImage{}", QUrl()); // TODO: depends on self text
  if (!component.isReady())
    qCritical() << "Component for RCTImageViewManager not ready";

  QQuickItem* item = qobject_cast<QQuickItem*>(component.create());
  if (item == nullptr) {
    qCritical() << "Unable to create component for RCTImageViewManager";
    return nullptr;
  }

  applyProperties(item, properties);

  return item;
}

void ReactImageManager::applyProperties(QQuickItem* item, const QVariantMap& properties) const
{
  if (properties.isEmpty())
    return;

  ReactViewManager::applyProperties(item, properties);

  for (const QString& key : properties.keys()) {
    if (key == "source") {
      QVariantMap si = properties.value("source").toMap();
      item->setProperty("source", si.value("uri"));
    }
  }
}
