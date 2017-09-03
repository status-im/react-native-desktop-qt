
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

void ReactRawTextManager::configureView(QQuickItem* view) const
{
  ReactViewManager::configureView(view);
  view->setEnabled(false);
}

QString ReactRawTextManager::qmlComponentFile() const
{
  return ":/qml/ReactRawText.qml";
}

