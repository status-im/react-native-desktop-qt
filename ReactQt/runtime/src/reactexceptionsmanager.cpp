
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

#include "reactexceptionsmanager.h"

#include "reactbridge.h"
#include "reactredboxitem.h"


class ReactExceptionsManagerPrivate {
public:
  ReactBridge* bridge = nullptr;
};


void ReactExceptionsManager::reportSoftException(
  const QString& message,
  const QList<QVariantMap>& stack,
  int exceptionId
) {
  d_func()->bridge->redbox()->showErrorMessage(message, stack);
}

void ReactExceptionsManager::reportFatalException(
  const QString& message,
  const QList<QVariantMap>& stack,
  int exceptionId
) {
  d_func()->bridge->redbox()->showErrorMessage(message, stack);

  // XXX: leaving out reload attempts for now
}

void ReactExceptionsManager::updateExceptionMessage(
  const QString& message,
  const QList<QVariantMap>& stack,
  int exceptionId
) {
  d_func()->bridge->redbox()->updateErrorMessage(message, stack);
}

void ReactExceptionsManager::reportUnhandledException(
  const QString& message,
  const QList<QVariantMap>& stack
) {
  reportFatalException(message, stack, 1);
}


ReactExceptionsManager::ReactExceptionsManager(QObject* parent)
  : QObject(parent)
  , d_ptr(new ReactExceptionsManagerPrivate)
{
}

ReactExceptionsManager::~ReactExceptionsManager()
{
}

void ReactExceptionsManager::setBridge(ReactBridge* bridge)
{
  Q_D(ReactExceptionsManager);
  d->bridge = bridge;
}

ReactViewManager* ReactExceptionsManager::viewManager()
{
  return nullptr;
}

QString ReactExceptionsManager::moduleName()
{
  return "RCTExceptionsManager";
}

QList<ReactModuleMethod*> ReactExceptionsManager::methodsToExport()
{
  return QList<ReactModuleMethod*>{};
}

QVariantMap ReactExceptionsManager::constantsToExport()
{
  return QVariantMap{};
}

