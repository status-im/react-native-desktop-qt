
#include <iterator>
#include <algorithm>

#include <QMetaObject>
#include <QMetaMethod>
#include <QJsonDocument>
#include <QQuickItem>

#include <QDebug>

#include "reactbridge.h"
#include "reactcomponentdata.h"
#include "reactmoduledata.h"
#include "reactattachedproperties.h"
#include "ubuntuuimanager.h"
#include "ubuntuviewmanager.h"


int UbuntuUIManager::m_nextRootTag = 1;


UbuntuUIManager::UbuntuUIManager()
  : m_bridge(nullptr)
{
}

UbuntuUIManager::~UbuntuUIManager()
{
}

void UbuntuUIManager::updateView
(
  int reactTag,
  const QString& viewName,
  const QVariantMap& properties
)
{
  qDebug() << __PRETTY_FUNCTION__ << reactTag << viewName << properties;

  // set properties on iew
  QQuickItem* item = m_views.value(reactTag);
  if (item == nullptr) {
    qWarning() << "Attempting to update properties on unknown view";
    return;
  }

  item->property("viewManager").value<UbuntuViewManager*>()->applyProperties(item, properties);
  // XXX:
  m_bridge->visualParent()->polish();
}

QList<QQuickItem*> indexedChildren(QQuickItem* parent, const QList<int>& indices)
{
  return QList<QQuickItem*>{};
}

void UbuntuUIManager::manageChildren
(
  int containerReactTag,
  const QList<int>& moveFromIndicies,
  const QList<int>& moveToIndices,
  const QList<int>& addChildReactTags,
  const QList<int>& addAtIndices,
  const QList<int>& removeAtIndices
)
{
  qDebug() << __PRETTY_FUNCTION__ << containerReactTag << moveFromIndicies << moveToIndices << addChildReactTags << addAtIndices << removeAtIndices;

  QQuickItem* container = m_views[containerReactTag];

  qDebug() << __PRETTY_FUNCTION__ << "container=" << container;
  QList<QQuickItem*> children;

  // removeAtIndices get unpluged and erased
  //  children = indexedChildren(container, removeAtIndices);

  // XXX: Assumption - addChildReactTags is sorted
  std::transform(addChildReactTags.begin(), addChildReactTags.end(),
                 std::back_inserter(children),
                 [this](int key) {
                   return m_views.value(key);
                 });

  // on iOS, order of the subviews implies z-order
  // implicitly its the same in QML, barring some exceptions.
  // XXX: existing views
  for (int i = 0; i < children.size(); ++i) {
    QQuickItem* child = children.at(i);
    child->setZ(i);
    child->setParentItem(container);
  }

  container->polish();
}

// Reacts version of first responder
void UbuntuUIManager::setJSResponder(int reactTag, bool blockNativeResponder)
{
  Q_UNUSED(reactTag);
  Q_UNUSED(blockNativeResponder);

  qDebug() << __PRETTY_FUNCTION__;
}

void UbuntuUIManager::clearJSResponder()
{
}

// in iOS, resign first responder (actual)
void UbuntuUIManager::blur(int reactTag)
{
  Q_UNUSED(reactTag);

  qDebug() << __PRETTY_FUNCTION__;
}

void UbuntuUIManager::createView
(
  int reactTag,
  const QString& viewName,
  int rootTag,
  const QVariantMap& props
)
{
  Q_UNUSED(rootTag);

  qDebug() << __PRETTY_FUNCTION__ << reactTag << viewName << rootTag; // << props;
  ReactComponentData* cd = m_componentData.value(viewName);
  if (cd == nullptr) {
    qCritical() << "Attempt to create unknow view of type" << viewName;
    return;
  }

  QQuickItem* item = cd->createView(reactTag, props);
  if (item == nullptr)
    return;

  ReactAttachedProperties* properties = ReactAttachedProperties::get(item);
  properties->setTag(reactTag);

  // XXX:
  item->setProperty("viewManager", QVariant::fromValue<UbuntuViewManager*>(cd->manager()));

  // XXX:
  m_views.insert(reactTag, item);
}


void UbuntuUIManager::setBridge(ReactBridge* bridge)
{
  qDebug() << __PRETTY_FUNCTION__;
  if (m_bridge != nullptr) {
    qCritical() << "Bridge already set, UIManager already initialised?";
    return;
  }

  m_bridge = bridge;

  for (ReactModuleData* data : m_bridge->modules()) {
    UbuntuViewManager* manager = data->viewManager();
    if (manager != nullptr) {
      ReactComponentData* cd = new ReactComponentData(manager);
      m_componentData.insert(cd->name(), cd);
    }
  }

  connect(m_bridge->visualParent(), SIGNAL(widthChanged()), SLOT(rootViewWidthChanged()));
  connect(m_bridge->visualParent(), SIGNAL(heightChanged()), SLOT(rootViewHeightChanged()));
  connect(m_bridge->visualParent(), SIGNAL(scaleChanged()), SLOT(rootViewScaleChanged()));
}

QString UbuntuUIManager::moduleName()
{
  return "RCTUIManager";
}

QStringList UbuntuUIManager::methodsToExport()
{
  const QMetaObject* metaObject = this->metaObject();
  const int methodCount = metaObject->methodCount();

  QStringList methods;
  for (int i = metaObject->methodOffset(); i < methodCount; ++i) {
    methods << metaObject->method(i).name();
  }
  return methods;
}

QVariantMap UbuntuUIManager::constantsToExport()
{
  QVariantMap rc;
  QVariantMap directEvents;
  QVariantMap bubblingEvents;

  for (const ReactComponentData* componentData : m_componentData) {
    qDebug() << "Checking" << componentData->name();

    QVariantMap managerInfo;

    QVariantMap config = componentData->viewConfig();
    if (!config.isEmpty()) {
      managerInfo.insert("NativeProps", config["propTypes"]);
    }

    for (const QString& eventName : config["directEvents"].toStringList()) {
      if (!directEvents.contains(eventName)) {
        QString tmp = eventName; tmp.replace(0, 3, "on");
        directEvents.insert(eventName,
                            QVariantMap{{"registrationName", tmp}});
      }
    }

    for (const QString& eventName : config["bubblingEvents"].toStringList()) {
      if (!bubblingEvents.contains(eventName)) {
        QString tmp = eventName; tmp.replace(0, 3, "on");
        bubblingEvents.insert(eventName,
                              QVariantMap{{"phasedRegistrationNames",
                                  QVariantMap{{"bubbled", tmp},
                                              {"captured", tmp.append("Capture")}}}});
      }
    }

    rc.insert(componentData->name(), managerInfo);
  }

  rc.insert("customBubblingEventTypes", bubblingEvents);
  rc.insert("customDirectEventTypes", directEvents);
  rc.insert("Dimensions",
            QVariantMap{
              { "width", m_bridge->visualParent()->width() },
              { "height", m_bridge->visualParent()->height() },
              { "scale", m_bridge->visualParent()->scale() }});
  rc.insert("modalFullscreenView",
            QVariantMap{
              { "width", m_bridge->visualParent()->width() },
              { "height", m_bridge->visualParent()->height() }});

  return rc;
}

int UbuntuUIManager::allocateRootTag()
{
  int tag = m_nextRootTag;
  m_nextRootTag += 10;
  return tag;
}

void UbuntuUIManager::registerRootView(QQuickItem* root)
{
  ReactAttachedProperties* properties = ReactAttachedProperties::get(root);
  m_views.insert(properties->tag(), root);
}

void UbuntuUIManager::rootViewWidthChanged()
{
  m_bridge->visualParent()->polish();
}

void UbuntuUIManager::rootViewHeightChanged()
{
  m_bridge->visualParent()->polish();
}

void UbuntuUIManager::rootViewScaleChanged()
{
  m_bridge->visualParent()->polish();
}
