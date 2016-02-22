#include <QString>
#include <QVariant>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQmlProperty>

#include <QDebug>

#include "reacttextproperties.h"
#include "reacttextmanager.h"
#include "reactbridge.h"
#include "reactflexlayout.h"


ReactTextManager::ReactTextManager(QObject* parent)
  : ReactRawTextManager(parent)
{
}

ReactTextManager::~ReactTextManager()
{
}

void ReactTextManager::setBridge(ReactBridge* bridge)
{
  m_bridge = bridge;
}

ReactViewManager* ReactTextManager::viewManager()
{
  return this;
}

ReactPropertyHandler* ReactTextManager::propertyHandler(QObject* object)
{
  Q_ASSERT(qobject_cast<QQuickItem*>(object) != nullptr);
  return ReactTextProperties::get(qobject_cast<QQuickItem*>(object));
}

QString ReactTextManager::moduleName()
{
  return "RCTTextManager";
}

QList<ReactModuleMethod*> ReactTextManager::methodsToExport()
{
  return QList<ReactModuleMethod*>{};
}

QVariantMap ReactTextManager::constantsToExport()
{
  return QVariantMap{};
}

namespace {
static const char* component_qml =
"import QtQuick 2.4\n"
"\n"
"Text {\n"
"  textFormat: Text.RichText\n"
"}\n";
}

bool ReactTextManager::shouldLayout() const
{
  return true;
}

QQuickItem* ReactTextManager::view(const QVariantMap& properties) const
{
  QString componentString = QString(component_qml);

  QQmlComponent component(m_bridge->qmlEngine());
  component.setData(componentString.toLocal8Bit(), QUrl());
  if (!component.isReady())
    qCritical() << "Component for RCTTextManager not ready";

  QQuickItem* item = qobject_cast<QQuickItem*>(component.create());
  if (item == nullptr) {
    qCritical() << "Unable to create component for RCTTextManager";
    return nullptr;
  }

  // item->setEnabled(false);

  return item;
}
