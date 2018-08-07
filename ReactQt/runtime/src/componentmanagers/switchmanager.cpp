
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
#include "layout/flexbox.h"
#include "propertyhandler.h"
#include "switchmanager.h"
#include "utilities.h"

const QString EVENT_ON_CHANGE = "onChange";

class SwitchManagerPrivate {};

SwitchManager::SwitchManager(QObject* parent) : ViewManager(parent), d_ptr(new SwitchManagerPrivate) {}

SwitchManager::~SwitchManager() {}

ViewManager* SwitchManager::viewManager() {
    return this;
}

QString SwitchManager::moduleName() {
    return "RCTSwitchManager";
}

QString SwitchManager::qmlComponentFile(const QVariantMap& properties) const {
    return "qrc:/qml/ReactSwitch.qml";
}

void SwitchManager::configureView(QQuickItem* view) const {
    ViewManager::configureView(view);
    view->setProperty("switchManager", QVariant::fromValue((QObject*)this));
    updateMeasureFunction(view);
}

void SwitchManager::updateMeasureFunction(QQuickItem* view) const {
    Flexbox* flexbox = Flexbox::findFlexbox(view);
    if (!flexbox) {
        return;
    }

    flexbox->setMeasureFunction([=](YGNodeRef, float, YGMeasureMode, float, YGMeasureMode) {
        float w = view->width();
        float h = view->height();
        return YGSize{w, h};
    });
}

QStringList SwitchManager::customDirectEventTypes() {
    return QStringList{};
}

bool SwitchManager::shouldLayout() const {
    return true;
}

void SwitchManager::sendValueChangeToJs(QQuickItem* switchItem, bool value) {
    if (!switchItem)
        return;

    notifyJsAboutEvent(tag(switchItem), EVENT_ON_CHANGE, QVariantMap{{"value", value}});
}

#include "switchmanager.moc"
