
/**
 * Copyright (C) 2016, Canonical Ltd.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * Author: Justin McPherson <justin.mcpherson@canonical.com>
 *
 */

#include <QString>
#include <QVariant>
#include <QQmlComponent>
#include <QQuickItem>

#include <QDebug>

#include "ubuntunavigatormanager.h"
#include "reactbridge.h"
#include "reactuimanager.h"
#include "reactattachedproperties.h"
#include "reactpropertyhandler.h"
#include "reactevents.h"


class NavigatorPropertyHandler : public ReactPropertyHandler {
  Q_OBJECT
  Q_PROPERTY(bool onBackButtonPress READ onBackButtonPress WRITE setOnBackButtonPress)
public:
  NavigatorPropertyHandler(QObject* object)
    : ReactPropertyHandler(object)
    , m_backButtonPress(false) {
    }
  bool onBackButtonPress() const;
  void setOnBackButtonPress(bool backButtonPress);

  bool m_backButtonPress;
};

bool NavigatorPropertyHandler::onBackButtonPress() const
{
  return m_backButtonPress;
}

void NavigatorPropertyHandler::setOnBackButtonPress(bool backButtonPress)
{
  m_backButtonPress = backButtonPress;
}



void UbuntuNavigatorManager::push(int containerTag, int viewTag)
{
  QQuickItem* container = m_bridge->uiManager()->viewForTag(containerTag);
  QQuickItem* view = m_bridge->uiManager()->viewForTag(viewTag);
  if (container == nullptr || view == nullptr) {
    qCritical() << __PRETTY_FUNCTION__ << "Invalid react tag passed";
    return;
  }

  invokeMethod("push(QVariant)", container, QVariantList{QVariant::fromValue(view)});
}

void UbuntuNavigatorManager::pop(int containerTag)
{
  QQuickItem* container = m_bridge->uiManager()->viewForTag(containerTag);
  if (container == nullptr) {
    qCritical() << __PRETTY_FUNCTION__ << "Invalid react tag passed";
    return;
  }
  invokeMethod("pop()", container);
}

void UbuntuNavigatorManager::clear(int containerTag)
{
  QQuickItem* container = m_bridge->uiManager()->viewForTag(containerTag);
  if (container == nullptr) {
    qCritical() << __PRETTY_FUNCTION__ << "Invalid react tag passed";
    return;
  }
  invokeMethod("clear()", container);
}


UbuntuNavigatorManager::UbuntuNavigatorManager(QObject* parent)
  : ReactViewManager(parent)
  , m_id(0)
{
}

UbuntuNavigatorManager::~UbuntuNavigatorManager()
{
}

void UbuntuNavigatorManager::setBridge(ReactBridge* bridge)
{
  m_bridge = bridge;
}

ReactViewManager* UbuntuNavigatorManager::viewManager()
{
  return this;
}

ReactPropertyHandler* UbuntuNavigatorManager::propertyHandler(QObject* object)
{
  Q_ASSERT(qobject_cast<QQuickItem*>(object) != nullptr);
  return new NavigatorPropertyHandler(object);
}

QString UbuntuNavigatorManager::moduleName()
{
  return "UbuntuNavigatorManager";
}

QList<ReactModuleMethod*> UbuntuNavigatorManager::methodsToExport()
{
  return QList<ReactModuleMethod*>{};
}

QVariantMap UbuntuNavigatorManager::constantsToExport()
{
  return QVariantMap{};
}

QStringList UbuntuNavigatorManager::customBubblingEventTypes()
{
  return QStringList{ normalizeInputEventName("onBackButtonPress") };
}

namespace {
static const char* component_qml = R"COMPONENT(
import QtQuick 2.4
import Ubuntu.Components 1.2

MainView {
  id: mainView%1
  property int numberPages: 0
  signal backTriggered();
  Component {
    id: pageBackAction%1
    Action {
      iconName: mainView%1.numberPages > 1 ? "back" : ""
    }
  }
  PageStack {
    id: pageStack%1
    anchors.fill: parent
  }
  function push(item) {
    item.head.backAction = pageBackAction%1.createObject(item);
    item.head.backAction.onTriggered.connect(backTriggered);
    pageStack%1.push(item);
    mainView%1.numberPages += 1;
  }
  function pop() {
    pageStack%1.pop();
    mainView%1.numberPages -= 1;
  }
  function clear() {
    pageStack%1.clear();
  }
}
)COMPONENT";
}

QQuickItem* UbuntuNavigatorManager::view(const QVariantMap& properties) const
{
  QString componentString = QString(component_qml).arg(m_id++);

  QQmlComponent component(m_bridge->qmlEngine());
  component.setData(componentString.toLocal8Bit(), QUrl());
  if (!component.isReady())
    qCritical() << "Component for UbuntuNavigatorManager not ready" <<
              componentString << component.errors();

  QQuickItem* item = qobject_cast<QQuickItem*>(component.create());
  if (item == nullptr) {
    qCritical() << "Unable to create component for UbuntuNavigatorManager";
    return nullptr;
  }

  configureView(item);

  return item;
}

void UbuntuNavigatorManager::backTriggered()
{
  ReactAttachedProperties* ap = ReactAttachedProperties::get(qobject_cast<QQuickItem*>(sender()));
  if (ap == nullptr) {
    qCritical() << __PRETTY_FUNCTION__ << "failed to find ReactAttachedProperties";
    return;
  }
  NavigatorPropertyHandler* ph = qobject_cast<NavigatorPropertyHandler*>(ap->propertyHandler());
  if (ph == nullptr) {
    qCritical() << __PRETTY_FUNCTION__ << "failed to find NavigatorPropertyHandler";
    return;
  }
  if (ph->onBackButtonPress()) {
    m_bridge->enqueueJSCall("RCTEventEmitter", "receiveEvent",
                            QVariantList{ap->tag(),
                                         normalizeInputEventName("onBackButtonPress"),
                                         QVariantMap{}});
  }
}

void UbuntuNavigatorManager::configureView(QQuickItem* view) const
{
  connect(view, SIGNAL(backTriggered()), SLOT(backTriggered()));
}

#define _R_ARG(argn) QGenericArgument(argn.typeName(), argn.data())
void UbuntuNavigatorManager::invokeMethod
(
  const QString& methodSignature,
  QQuickItem* item,
  const QVariantList& args
) {
  QMetaMethod m = findMethod(methodSignature, item);
  if (!m.isValid()) {
    qCritical() << __PRETTY_FUNCTION__ << "Attempting to invoke unknown method";
    return;
  }

  // XXX: share this machinery somehow
  switch (args.size()) {
    case 0:
    m.invoke(item, Qt::DirectConnection);
    break;
  case 1:
    m.invoke(item, Qt::DirectConnection, _R_ARG(args.at(0)));
    break;
  }
}

QMetaMethod UbuntuNavigatorManager::findMethod
(
  const QString& methodSignature,
  QQuickItem* item
) {
  QPair<QString, QQuickItem*> mp = qMakePair(methodSignature, item);
  if (m_methodCache.find(mp) != m_methodCache.end()) {
    return m_methodCache.value(mp);
  } else {
    const QMetaObject* metaObject = item->metaObject();
    const int methodCount = metaObject->methodCount();

    for (int i = metaObject->methodOffset(); i < methodCount; ++i) {
      QMetaMethod m = metaObject->method(i);
      if (m.methodSignature() == methodSignature.toLocal8Bit()) {
        m_methodCache.insert(qMakePair(methodSignature, item), m);
        return m;
      }
    }
  }

  return QMetaMethod();
}

#include "ubuntunavigatormanager.moc"
