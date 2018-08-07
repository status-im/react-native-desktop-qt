
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
#include "pickermanager.h"
#include "propertyhandler.h"
#include "utilities.h"

const QString EVENT_ON_VALUE_CHANGE = "onValueChange";

class PickerManagerPrivate {};

PickerManager::PickerManager(QObject* parent) : ViewManager(parent), d_ptr(new PickerManagerPrivate) {}

PickerManager::~PickerManager() {}

ViewManager* PickerManager::viewManager() {
    return this;
}

QString PickerManager::moduleName() {
    return "RCTPickerViewManager";
}

QString PickerManager::qmlComponentFile(const QVariantMap& properties) const {
    return "qrc:/qml/ReactPicker.qml";
}

void PickerManager::configureView(QQuickItem* view) const {
    ViewManager::configureView(view);
    view->setProperty("pickerManager", QVariant::fromValue((QObject*)this));
}

QStringList PickerManager::customDirectEventTypes() {
    return QStringList{EVENT_ON_VALUE_CHANGE};
}

bool PickerManager::shouldLayout() const {
    return true;
}

void PickerManager::sendValueChangeToJs(QQuickItem* picker, int index) {
    if (!picker)
        return;

    notifyJsAboutEvent(tag(picker), EVENT_ON_VALUE_CHANGE, QVariantMap{{"target", tag(picker)}, {"position", index}});
}

#include "pickermanager.moc"
