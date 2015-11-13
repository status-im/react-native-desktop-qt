
#include <QMetaObject>
#include <QMetaMethod>
#include <QJsonDocument>
#include <QQuickItem>

#include <QDebug>

#include "reactbridge.h"
#include "reactcomponentdata.h"
#include "reactmoduledata.h"
#include "ubuntuuimanager.h"
#include "ubuntuviewmanager.h"



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
) {
}

void UbuntuUIManager::manageChildren
(
  int containerReactTag,
  const QVariantList& moveFromIndicies,
  const QVariantList& moveToIndices,
  const QVariantList& addChildReactTags,
  const QVariantList& addAtIndices,
  const QVariantList& removeAtIndices
) {
  qDebug() << __PRETTY_FUNCTION__ << containerReactTag << moveFromIndicies << moveToIndices << addChildReactTags << addAtIndices << removeAtIndices;
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

  item->setProperty("reactTag", reactTag);
  item->setParentItem(m_bridge->visualParent());
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
