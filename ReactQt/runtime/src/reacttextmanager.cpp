
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
static const char* component_qml = R"COMPONENT(
import QtQuick 2.4

Text {
  textFormat: Text.RichText
  wrapMode: Text.WordWrap
}
)COMPONENT";
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
    qCritical() << "Component for RCTTextManager not ready" << component.errors();

  QQuickItem* item = qobject_cast<QQuickItem*>(component.create());
  if (item == nullptr) {
    qCritical() << "Unable to create component for RCTTextManager";
    return nullptr;
  }

  return item;
}
