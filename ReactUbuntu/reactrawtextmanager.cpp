#include <QString>
#include <QVariant>
#include <QQuickItem>
#include <QQmlProperty>

#include "reacttextproperties.h"
#include "reactrawtextmanager.h"
#include "reactbridge.h"
#include "reactpropertyhandler.h"


ReactRawTextManager::ReactRawTextManager(QObject *parent)
  : ReactViewManager(parent)
{
}

ReactRawTextManager::~ReactRawTextManager()
{
}

void ReactRawTextManager::setBridge(ReactBridge* bridge)
{
  m_bridge = bridge;
}

// TODO: this doesnt seem right
ReactViewManager* ReactRawTextManager::viewManager()
{
  return this;
}

ReactPropertyHandler* ReactRawTextManager::propertyHandler(QObject* object)
{
  Q_ASSERT(qobject_cast<QQuickItem*>(object) != nullptr);
  return ReactTextProperties::get(qobject_cast<QQuickItem*>(object));
}

QString ReactRawTextManager::moduleName()
{
  return "RCTRawTextManager";
}

QList<ReactModuleMethod*> ReactRawTextManager::methodsToExport()
{
  return QList<ReactModuleMethod*>{};
}

QVariantMap ReactRawTextManager::constantsToExport()
{
  return QVariantMap{};
}

namespace {
static const char* component_qml =
"import QtQuick 2.4\n"
"\n"
"Text {\n"
"  horizontalAlignment: Text.AlignHCenter\n"
"  verticalAlignment: Text.AlignVCenter\n"
"}\n";
}

QQuickItem* ReactRawTextManager::view(const QVariantMap& properties) const
{
  QString componentString = QString(component_qml);

  QQmlComponent component(m_bridge->qmlEngine());
  component.setData(componentString.toLocal8Bit(), QUrl());
  if (!component.isReady())
    qCritical() << "Component for RCTRawTextManager not ready";

  QQuickItem* item = qobject_cast<QQuickItem*>(component.create());
  if (item == nullptr) {
    qCritical() << "Unable to create component for RCTRawTextManager";
    return nullptr;
  }

  //
  item->setEnabled(false);

  return item;
}

// void ReactRawTextManager::applyProperties(QQuickItem* item, const QVariantMap& properties) const
// {
//   // qDebug() << __PRETTY_FUNCTION__ << item << properties;
// 
//   if (properties.isEmpty())
//     return;
// 
//   ReactViewManager::applyProperties(item, properties);
// 
//   if (properties.contains("text")) {
//     item->setProperty("text", properties.value("text"));
//   }
// 
//   // Grab style from parent text item
//   // XXX: we can most likely not apply these sensibly the first time
//   // because the item has not been added to the visual hierarchy
//   ReactTextProperties *rtp = ReactTextProperties::get(item->parentItem(), false);
//   if (rtp == nullptr)
//     return; // XXX: prob should travel back up to the root text node
// 
// 
//   item->setProperty("color", rtp->color());
// 
//   if (rtp->numberOfLines() != -1) {
//     item->setProperty("maximumLineCount", rtp->numberOfLines());
//   }
// }

#include "reactrawtextmanager.moc"
