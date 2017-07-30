
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

#include "reacttextproperties.h"
#include "reactrawtextmanager.h"
#include "reactbridge.h"
#include "reactrawtextproperties.h"




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
  return ReactRawTextProperties::get(qobject_cast<QQuickItem*>(object));
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

bool ReactRawTextManager::shouldLayout() const
{
  return false;
}

// TODO: this is a virtual node, not a real text node
namespace {
static const char* component_qml = R"COMPONENT(
import QtQuick 2.4

Text {
  visible: false
}
)COMPONENT";
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

