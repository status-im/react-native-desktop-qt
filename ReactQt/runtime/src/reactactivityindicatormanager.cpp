
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include <QQmlComponent>
#include <QQmlProperty>
#include <QQuickItem>
#include <QString>
#include <QVariant>

#include <QDebug>

#include "reactactivityindicatormanager.h"
#include "reactattachedproperties.h"
#include "reactbridge.h"
#include "reactpropertyhandler.h"
#include "utilities.h"

class ReactActivityIndicatorManagerPrivate {};

ReactActivityIndicatorManager::ReactActivityIndicatorManager(QObject* parent)
    : ReactViewManager(parent), d_ptr(new ReactActivityIndicatorManagerPrivate) {}

ReactActivityIndicatorManager::~ReactActivityIndicatorManager() {}

ReactViewManager* ReactActivityIndicatorManager::viewManager() {
    return this;
}

QString ReactActivityIndicatorManager::moduleName() {
    return "RCTActivityIndicatorViewManager";
}

QString ReactActivityIndicatorManager::qmlComponentFile() const {
    return ":/qml/ReactActivityIndicator.qml";
}

#include "reactactivityindicatormanager.moc"
