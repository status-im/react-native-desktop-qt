
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

#include <QGuiApplication>

#include <QDebug>

#include "appstate.h"

namespace {
QString reactAppState(Qt::ApplicationState state) {
    switch (state) {
    case Qt::ApplicationSuspended:
    case Qt::ApplicationHidden:
        return "background";
    case Qt::ApplicationInactive:
        return "inactive";
    case Qt::ApplicationActive:
        return "active";
    }
}
} // namespace

void AppState::getCurrentAppState(const ModuleInterface::ListArgumentBlock& success,
                                  const ModuleInterface::MapArgumentBlock& error) {
    success(m_bridge, QVariantList{QVariantMap{{"app_state", reactAppState(QGuiApplication::applicationState())}}});
}

AppState::AppState(QObject* parent) : QObject(parent) {}

AppState::~AppState() {}

void AppState::setBridge(Bridge* bridge) {
    m_bridge = bridge;
}

ViewManager* AppState::viewManager() {
    return nullptr;
}

QString AppState::moduleName() {
    return "RCTAppState";
}
