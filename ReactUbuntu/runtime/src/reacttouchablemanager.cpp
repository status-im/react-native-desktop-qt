
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

#include "reacttouchablemanager.h"
#include "reactbridge.h"


ReactTouchableManager::ReactTouchableManager(QObject* parent)
  : ReactViewManager(parent)
  , m_id(0)
{
}

ReactTouchableManager::~ReactTouchableManager()
{
}

void ReactTouchableManager::setBridge(ReactBridge* bridge)
{
  m_bridge = bridge;
}

ReactViewManager* ReactTouchableManager::viewManager()
{
  return this;
}

QString ReactTouchableManager::moduleName()
{
  return "ReactTouchableManager";
}

QList<ReactModuleMethod*> ReactTouchableManager::methodsToExport()
{
  return QList<ReactModuleMethod*>{};
}

QVariantMap ReactTouchableManager::constantsToExport()
{
  return QVariantMap{};
}

namespace {
static const char* component_qml =
"import QtQuick 2.4\n"
"\n"
"Rectangle {\n"
"  signal clicked()"
"  color: \"green\"\n"
"  MouseArea {\n"
"    id: touchableMouseId%1\n"
"    anchors.fill: parent\n"
"  }\n"
"  Connections {\n"
"    target: touchableMouseId%1\n"
"    onClicked: clicked()\n"
"  }\n"
"}\n";
}

QQuickItem* ReactTouchableManager::view(const QVariantMap& properties) const
{
  QString componentString = QString(component_qml).arg(m_id++);

  QQmlComponent component(m_bridge->qmlEngine());
  component.setData(componentString.toLocal8Bit(), QUrl());
  if (!component.isReady())
    qCritical() << "Component for ReactTouchableManager not ready";

  QQuickItem* item = qobject_cast<QQuickItem*>(component.create());
  if (item == nullptr) {
    qCritical() << "Unable to create component for ReactTouchableManager";
    return nullptr;
  }

  configureView(item);
  applyProperties(item, properties);

  return item;
}

void ReactTouchableManager::applyProperties(QQuickItem* item, const QVariantMap& properties) const
{
  if (properties.isEmpty())
    return;

  ReactViewManager::applyProperties(item, properties);
}

void ReactTouchableManager::clicked()
{
  qDebug() << __PRETTY_FUNCTION__;
}

void ReactTouchableManager::configureView(QQuickItem* view) const
{
  connect(view, SIGNAL(clicked()), SLOT(clicked()));
}
