
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

#include "exceptionsmanager.h"

#include "bridge.h"
#include "redbox.h"

class ExceptionsManagerPrivate {
public:
    Bridge* bridge = nullptr;
};

void ExceptionsManager::reportSoftException(const QString& message, const QList<QVariantMap>& stack, int exceptionId) {
    d_func()->bridge->redbox()->showErrorMessage(message, stack);
}

void ExceptionsManager::reportFatalException(const QString& message, const QList<QVariantMap>& stack, int exceptionId) {
    d_func()->bridge->redbox()->showErrorMessage(message, stack);

    // XXX: leaving out reload attempts for now
}

void ExceptionsManager::updateExceptionMessage(const QString& message,
                                               const QList<QVariantMap>& stack,
                                               int exceptionId) {
    d_func()->bridge->redbox()->updateErrorMessage(message, stack);
}

void ExceptionsManager::reportUnhandledException(const QString& message, const QList<QVariantMap>& stack) {
    reportFatalException(message, stack, 1);
}

ExceptionsManager::ExceptionsManager(QObject* parent) : QObject(parent), d_ptr(new ExceptionsManagerPrivate) {}

ExceptionsManager::~ExceptionsManager() {}

void ExceptionsManager::setBridge(Bridge* bridge) {
    Q_D(ExceptionsManager);
    d->bridge = bridge;
}

ViewManager* ExceptionsManager::viewManager() {
    return nullptr;
}

QString ExceptionsManager::moduleName() {
    return "RCTExceptionsManager";
}
