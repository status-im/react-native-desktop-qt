
#include <QString>
#include <QVariant>
#include <QQmlComponent>
#include <QQuickItem>

#include <QDebug>

#include "reactattachedproperties.h"
#include "reactevents.h"
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

QStringList UbuntuScrollViewManager::customDirectEventTypes()
{
  return QStringList{"scrollBeginDrag", "scroll", "scrollEndDrag", "scrollAnimationEnd",
                     "momentumScrollBegin", "momentumScrollEnd"};
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
  configureView(item);

  return item;
}

void UbuntuScrollViewManager::scrollBeginDrag()
{
  QQuickItem* item = qobject_cast<QQuickItem*>(sender());
  Q_ASSERT(item != nullptr);

  ReactAttachedProperties* rap = ReactAttachedProperties::get(item, false);
  if (rap == nullptr) {
    qCritical() << "Could not get reacTag for ScrollView!";
    return;
  }
  int reactTag = rap->tag();

  m_bridge->enqueueJSCall("RCTEventEmitter", "receiveEvent",
                          QVariantList{reactTag, normalizeInputEventName("scrollBeginDrag")});
}

void UbuntuScrollViewManager::scrollEndDrag()
{
  QQuickItem* item = qobject_cast<QQuickItem*>(sender());
  Q_ASSERT(item != nullptr);

  ReactAttachedProperties* rap = ReactAttachedProperties::get(item, false);
  if (rap == nullptr) {
    qCritical() << "Could not get reacTag for ScrollView!";
    return;
  }
  int reactTag = rap->tag();

  m_bridge->enqueueJSCall("RCTEventEmitter", "receiveEvent",
                          QVariantList{reactTag, normalizeInputEventName("scrollEndDrag")});
}

void UbuntuScrollViewManager::scroll()
{
  QQuickItem* item = qobject_cast<QQuickItem*>(sender());
  Q_ASSERT(item != nullptr);

  ReactAttachedProperties* rap = ReactAttachedProperties::get(item, false);
  if (rap == nullptr) {
    qCritical() << "Could not get reacTag for ScrollView!";
    return;
  }
  int reactTag = rap->tag();

  m_bridge->enqueueJSCall("RCTEventEmitter", "receiveEvent",
                          QVariantList{reactTag, normalizeInputEventName("scroll")});
}

void UbuntuScrollViewManager::momentumScrollBegin()
{
  QQuickItem* item = qobject_cast<QQuickItem*>(sender());
  Q_ASSERT(item != nullptr);

  ReactAttachedProperties* rap = ReactAttachedProperties::get(item, false);
  if (rap == nullptr) {
    qCritical() << "Could not get reacTag for ScrollView!";
    return;
  }
  int reactTag = rap->tag();

  m_bridge->enqueueJSCall("RCTEventEmitter", "receiveEvent",
                          QVariantList{reactTag, normalizeInputEventName("momentumScrollBegin")});
}

void UbuntuScrollViewManager::momentumScrollEnd()
{
  QQuickItem* item = qobject_cast<QQuickItem*>(sender());
  Q_ASSERT(item != nullptr);

  ReactAttachedProperties* rap = ReactAttachedProperties::get(item, false);
  if (rap == nullptr) {
    qCritical() << "Could not get reactTag for ScrollView!";
    return;
  }
  int reactTag = rap->tag();

  m_bridge->enqueueJSCall("RCTEventEmitter", "receiveEvent",
                          QVariantList{reactTag, normalizeInputEventName("momentumScrollEnd")});
}

void UbuntuScrollViewManager::configureView(QQuickItem* view) const
{
  // This would be prettier with a Functor version, but connect doesnt support it
  connect(view, SIGNAL(movementStarted()), SLOT(scrollBeginDrag()));
  connect(view, SIGNAL(movementEnded()), SLOT(scrollEndDrag()));
  connect(view, SIGNAL(movingChanged()), SLOT(scroll()));

  connect(view, SIGNAL(flickStarted()), SLOT(momentumScrollBegin()));
  connect(view, SIGNAL(flickEnded()), SLOT(momentumScrollEnd()));
}
