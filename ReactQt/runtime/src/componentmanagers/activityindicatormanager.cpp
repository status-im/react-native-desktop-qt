
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

#include "attachedproperties.h"
#include "bridge.h"
#include "componentmanagers/activityindicatormanager.h"
#include "propertyhandler.h"
#include "utilities.h"

class ActivityIndicatorManagerPrivate {};

ActivityIndicatorManager::ActivityIndicatorManager(QObject* parent)
    : ViewManager(parent), d_ptr(new ActivityIndicatorManagerPrivate) {}

ActivityIndicatorManager::~ActivityIndicatorManager() {}

ViewManager* ActivityIndicatorManager::viewManager() {
    return this;
}

QString ActivityIndicatorManager::moduleName() {
    return "RCTActivityIndicatorViewManager";
}

QString ActivityIndicatorManager::qmlComponentFile(const QVariantMap& properties) const {
    return "qrc:/qml/ReactActivityIndicator.qml";
}

#include "activityindicatormanager.moc"
