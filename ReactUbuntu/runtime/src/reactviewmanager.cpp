
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
#include <QQuickItem>
#include <QQmlProperty>

#include "reactviewmanager.h"
#include "reactitem.h"
#include "reactbridge.h"
#include "reactvaluecoercion.h"
#include "reactflexlayout.h"
#include "reactpropertyhandler.h"
#include "reacttextproperties.h"


ReactViewManager::ReactViewManager(QObject *parent)
  : QObject(parent)
{
}

ReactViewManager::~ReactViewManager()
{
}

void ReactViewManager::setBridge(ReactBridge* bridge)
{
  m_bridge = bridge;
}

// TODO: this doesnt seem right
ReactViewManager* ReactViewManager::viewManager()
{
  return this;
}

ReactPropertyHandler* ReactViewManager::propertyHandler(QObject* object)
{
  return new ReactPropertyHandler(object);
}

QString ReactViewManager::moduleName()
{
  return "RCTViewManager";
}

QList<ReactModuleMethod*> ReactViewManager::methodsToExport()
{
  return QList<ReactModuleMethod*>{};
}

QVariantMap ReactViewManager::constantsToExport()
{
  return QVariantMap{};
}

QStringList ReactViewManager::customDirectEventTypes()
{
  return QStringList{};
}

QStringList ReactViewManager::customBubblingEventTypes()
{
  return QStringList{"press", "change", "focus", "blur", "submitEditing", "endEditing", "touchStart", "touchMove", "touchCancel", "touchEnd"};
}

bool ReactViewManager::shouldLayout() const
{
  return true;
}

void ReactViewManager::addChildItem(QQuickItem* container, QQuickItem* child, int position) const
{
  // XXX: remove this
  if ((ReactTextProperties::get(container, false) == nullptr) &&
      (ReactTextProperties::get(child, false) != nullptr)) {
    ReactTextProperties::get(child)->hookLayout();
  }
  child->setParentItem(container);
  child->setZ(position);
}

namespace {
static const char* component_qml = R"COMPONENT(
import QtQuick 2.4
import React 0.1 as React
React.Item {
}
)COMPONENT";
}

QQuickItem* ReactViewManager::view(const QVariantMap& properties) const
{
  QQmlComponent component(m_bridge->qmlEngine());
  component.setData(component_qml, QUrl());
  if (!component.isReady())
    qCritical() << "React.Item is not ready!" << component.errors();

  QQuickItem* item = qobject_cast<QQuickItem*>(component.create());
  if (item == nullptr) {
    qCritical() << "Unable to construct React.Item";
    return nullptr;
  }

  return item;
}


#include "reactviewmanager.moc"
