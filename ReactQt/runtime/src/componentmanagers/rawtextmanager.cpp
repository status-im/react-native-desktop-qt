
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

#include <QQmlProperty>
#include <QQuickItem>
#include <QString>
#include <QVariant>

#include "bridge.h"
#include "propertyhandler.h"
#include "rawtextmanager.h"

RawTextManager::RawTextManager(QObject* parent) : ViewManager(parent) {}

RawTextManager::~RawTextManager() {}

// TODO: this doesnt seem right
ViewManager* RawTextManager::viewManager() {
    return this;
}

QString RawTextManager::moduleName() {
    return "RCTRawTextManager";
}

bool RawTextManager::shouldLayout() const {
    return false;
}

void RawTextManager::configureView(QQuickItem* view) const {
    ViewManager::configureView(view);
    view->setEnabled(false);
}

QString RawTextManager::qmlComponentFile(const QVariantMap& properties) const {
    return "qrc:/qml/ReactRawText.qml";
}
