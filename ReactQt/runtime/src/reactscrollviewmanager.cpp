
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

#include "reactattachedproperties.h"
#include "reactevents.h"
#include "reactscrollviewmanager.h"
#include "reactbridge.h"
#include "reactuimanager.h"
#include "reactpropertyhandler.h"
#include "reactevents.h"


class ScrollViewPropertyHandler : public ReactPropertyHandler {
  Q_OBJECT
  Q_PROPERTY(bool onScroll READ onScroll WRITE setOnScroll)
public:
  ScrollViewPropertyHandler(QObject* object)
    : ReactPropertyHandler(object) {
      m_onScroll = false;
    }
  bool onScroll() const;
  void setOnScroll(bool backButtonPress);

  bool m_onScroll;
};

bool ScrollViewPropertyHandler::onScroll() const
{
  return m_onScroll;
}

void ScrollViewPropertyHandler::setOnScroll(bool onScroll)
{
  m_onScroll = onScroll;
}


void ReactScrollViewManager::scrollTo(
  int reactTag,
  double offsetX,
  double offsetY,
  bool animated
) {
  QQuickItem* item = m_bridge->uiManager()->viewForTag(reactTag);
  Q_ASSERT(item != nullptr);

  QQmlProperty(item, "contentX").write(offsetX);
  QQmlProperty(item, "contentY").write(offsetY);
}


ReactScrollViewManager::ReactScrollViewManager(QObject* parent)
  : ReactViewManager(parent)
{
}

ReactScrollViewManager::~ReactScrollViewManager()
{
}

void ReactScrollViewManager::setBridge(ReactBridge* bridge)
{
  m_bridge = bridge;
}

ReactViewManager* ReactScrollViewManager::viewManager()
{
  return this;
}

ReactPropertyHandler* ReactScrollViewManager::propertyHandler(QObject* object)
{
  Q_ASSERT(qobject_cast<QQuickItem*>(object) != nullptr);
  return new ScrollViewPropertyHandler(object);
}

QString ReactScrollViewManager::moduleName()
{
  return "RCTScrollViewManager";
}

QList<ReactModuleMethod*> ReactScrollViewManager::methodsToExport()
{
  return QList<ReactModuleMethod*>{};
}

QVariantMap ReactScrollViewManager::constantsToExport()
{
  return QVariantMap{};
}

QStringList ReactScrollViewManager::customDirectEventTypes()
{
  return QStringList{"scrollBeginDrag", normalizeInputEventName("onScroll"), "scrollEndDrag", "scrollAnimationEnd",
                     "momentumScrollBegin", "momentumScrollEnd"};
}

void ReactScrollViewManager::addChildItem(QQuickItem* scrollView, QQuickItem* child , int position) const
{
  // add to parents content item
  QQuickItem* contentItem = QQmlProperty(scrollView, "contentItem").read().value<QQuickItem*>();
  Q_ASSERT(contentItem != nullptr);

  child->setParentItem(contentItem);  
  child->setProperty("index", position);
}

namespace {
static const char* component_qml = R"COMPONENT(
import QtQuick 2.4
import QtQuick.Controls 1.4

Flickable {
 id: scrollView
 clip: true
}

)COMPONENT";
}

QQuickItem* ReactScrollViewManager::view(const QVariantMap& properties) const
{
  QQmlComponent component(m_bridge->qmlEngine());
  component.setData(component_qml, QUrl());
  if (!component.isReady())
    qCritical() << "Component for ReactScrollView not ready";

  QQuickItem* item = qobject_cast<QQuickItem*>(component.create());
  if (item == nullptr) {
    qCritical() << "Unable to create component for ReactScrollViewManager";
    return nullptr;
  }

  configureView(item);

  return item;
}

void ReactScrollViewManager::scrollBeginDrag()
{
  // qDebug() << __PRETTY_FUNCTION__;
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

void ReactScrollViewManager::scrollEndDrag()
{
  // qDebug() << __PRETTY_FUNCTION__;
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

void ReactScrollViewManager::scroll()
{
  QQuickItem* item = qobject_cast<QQuickItem*>(sender());
  Q_ASSERT(item != nullptr);

  ReactAttachedProperties* ap = ReactAttachedProperties::get(item);
  if (ap == nullptr) {
    qCritical() << __PRETTY_FUNCTION__ << "failed to find ReactAttachedProperties";
    return;
  }
  ScrollViewPropertyHandler* ph = qobject_cast<ScrollViewPropertyHandler*>(ap->propertyHandler());
  if (ph == nullptr) {
    qCritical() << __PRETTY_FUNCTION__ << "failed to find ScrollViewPropertyHandler";
    return;
  }
  if (ph->onScroll()) {
    m_bridge->enqueueJSCall("RCTEventEmitter", "receiveEvent",
                            QVariantList{ap->tag(),
                                         normalizeInputEventName("onScroll"),
                                         buildEventData(item)});
  }
}

void ReactScrollViewManager::momentumScrollBegin()
{
  // qDebug() << __PRETTY_FUNCTION__;
  QQuickItem* item = qobject_cast<QQuickItem*>(sender());
  Q_ASSERT(item != nullptr);

  ReactAttachedProperties* rap = ReactAttachedProperties::get(item, false);
  if (rap == nullptr) {
    qCritical() << "Could not get reacTag for ScrollView!";
    return;
  }
  int reactTag = rap->tag();

  m_bridge->enqueueJSCall("RCTEventEmitter", "receiveEvent",
                          QVariantList{reactTag,
                                       normalizeInputEventName("momentumScrollBegin"),
                                       buildEventData(item)});
}

void ReactScrollViewManager::momentumScrollEnd()
{
  // qDebug() << __PRETTY_FUNCTION__;
  QQuickItem* item = qobject_cast<QQuickItem*>(sender());
  Q_ASSERT(item != nullptr);

  ReactAttachedProperties* rap = ReactAttachedProperties::get(item, false);
  if (rap == nullptr) {
    qCritical() << "Could not get reactTag for ScrollView!";
    return;
  }
  int reactTag = rap->tag();

  m_bridge->enqueueJSCall("RCTEventEmitter", "receiveEvent",
                          QVariantList{reactTag,
                                       normalizeInputEventName("momentumScrollEnd"),
                                       buildEventData(item)});
}

namespace {
template<typename TP>
TP propertyValue(QQuickItem* item, const QString& property)
{
  return QQmlProperty(item, property).read().value<TP>();
}
}

QVariantMap ReactScrollViewManager::buildEventData(QQuickItem* item) const
{
  QVariantMap ed;
  ed.insert("contentOffset", QVariantMap{
    { "x", propertyValue<double>(item, "contentX") },
    { "y", propertyValue<double>(item, "contentY") },
  });
  // ed.insert("contentInset", QVariantMap{
  // });
  ed.insert("contentSize", QVariantMap{
    { "width", propertyValue<double>(item, "contentWidth") },
    { "height", propertyValue<double>(item, "contentHeight") },
  });
  ed.insert("layoutMeasurement", QVariantMap{
    { "width", propertyValue<double>(item, "width") },
    { "height", propertyValue<double>(item, "height") },
  });
  ed.insert("zoomScale", 1);
  return ed;
}

void ReactScrollViewManager::configureView(QQuickItem* view) const
{
  // This would be prettier with a Functor version, but connect doesnt support it
  connect(view, SIGNAL(movementStarted()), SLOT(scrollBeginDrag()));
  connect(view, SIGNAL(movementEnded()), SLOT(scrollEndDrag()));
  connect(view, SIGNAL(movingChanged()), SLOT(scroll()));

  connect(view, SIGNAL(flickStarted()), SLOT(momentumScrollBegin()));
  connect(view, SIGNAL(flickEnded()), SLOT(momentumScrollEnd()));
}

#include "reactscrollviewmanager.moc"
