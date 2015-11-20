
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


int UbuntuUIManager::m_nextTag = 1;


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
) {
  qDebug() << __PRETTY_FUNCTION__ << containerReactTag << moveFromIndicies << moveToIndices << addChildReactTags << addAtIndices << removeAtIndices;

  QQuickItem* container = m_views[containerReactTag];

  QList<QQuickItem*> children;

  // removeAtIndices get unpluged and erased
  //  children = indexedChildren(container, removeAtIndices);

  // XXX: Assumption - addChildReactTags is sorted
  if (containerReactTag == 11) //XXX: !!
    container = m_bridge->visualParent();

  std::transform(addChildReactTags.begin(), addChildReactTags.end(),
                 std::back_inserter(children),
                 [this](int key) {
                   return m_views.value(key);
                 });

  // on iOS, order of the subviews implies z-order
  // XXX: existing views
  for (int i = 0; i < children.size(); ++i) {
    QQuickItem* child = children.at(i);
    child->setZ(i);
    child->setParentItem(container);
    child->property("viewManager").value<UbuntuViewManager*>()->
      updateLayout(child, child->property("reactProperties").toMap());
  }
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
  item->setProperty("reactProperties", props);
  item->setProperty("viewManager", QVariant::fromValue<UbuntuViewManager*>(cd->manager()));

  // XXX:
  m_views.insert(reactTag, item);

  // most likely we keep the properties then apply on manageChildren
}


void UbuntuUIManager::setBridge(ReactBridge* bridge)
{
  qDebug() << __PRETTY_FUNCTION__;
  if (m_bridge != nullptr) {
    qCritical() << "Bridge already set, UIManager already initialised?";
    return;
  }

  m_bridge = bridge;

  Q_FOREACH(ReactModuleData* data, m_bridge->modules()) {
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



int getWidth() {
  return 640;
}

int getHeight() {
  return 480;
}

int getScale()
{
  return 1;
}

QVariantMap UbuntuUIManager::constantsToExport()
{
  QVariantMap rc;
  QVariantMap directEvents;
  QVariantMap bubblingEvents;

  Q_FOREACH(const ReactComponentData* componentData, m_componentData) {
    qDebug() << "Checking" << componentData->name();

    QVariantMap managerInfo;

    QVariantMap config = componentData->viewConfig();
    if (!config.isEmpty()) {
      managerInfo.insert("NativeProps", config["propTypes"]);
    }

    Q_FOREACH(const QString& eventName, config["directEvents"].toStringList()) {
      QString tmp = eventName; tmp.replace(0, 3, "on");
      if (!directEvents.contains(eventName)) {
        directEvents.insert(eventName,
                            QVariantMap{{"registrationName", tmp}});
      }
    }

    Q_FOREACH(const QString& eventName, config["bubblingEvents"].toStringList()) {
      if (!bubblingEvents.contains(eventName)) {
        QString eventNameTmp = eventName; eventNameTmp.replace(0, 3, "on");
        QString bubbleName = eventName; bubbleName.replace(0, 3, "on");
        bubblingEvents.insert(eventName,
                              QVariantMap{{"phasedRegistrationNames",
                                  QVariantMap{{"bubbled", eventNameTmp.replace(0, 3, "on")},
                                              {"captured", bubbleName.append("Capture")}}}});
      }
    }

    rc.insert(componentData->name(), managerInfo);
  }

  rc.insert("customBubblingEventTypes", bubblingEvents);
  rc.insert("customDirectEventTypes", directEvents);
  rc.insert("Dimensions",
            QVariantMap{
              { "width", getWidth() },
              { "height", getHeight()},
              { "scale", getScale() }});
  rc.insert("modalFullscreenView",
            QVariantMap{
              { "width", getWidth() },
              { "height", getHeight()}});

  return rc;
}

int UbuntuUIManager::allocateRootTag()
{
  int tag = m_nextTag;
  m_nextTag += 10;
  return tag;
}

void UbuntuUIManager::registerRootView(QQuickItem* root)
{
  ReactAttachedProperties* properties = ReactAttachedProperties::get(root);
  m_views.insert(properties->tag(), root);
}

void UbuntuUIManager::rootViewWidthChanged()
{
}

void UbuntuUIManager::rootViewHeightChanged()
{
}

void UbuntuUIManager::rootViewScaleChanged()
{
}
