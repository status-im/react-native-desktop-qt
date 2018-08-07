
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
#include "buttonmanager.h"
#include "layout/flexbox.h"
#include "propertyhandler.h"
#include "utilities.h"
#include <functional>

using namespace utilities;

extern "C" {
#include "../../../ReactCommon/yoga/yoga/Yoga.h"
}

#include <QQuickItem>

const QString EVENT_ONPRESSED = "onPress";

class ButtonManagerPrivate {
public:
    QQuickItem* buttonTextControl(QQuickItem* button) const;
};

QQuickItem* ButtonManagerPrivate::buttonTextControl(QQuickItem* button) const {
    Q_ASSERT(button);
    QQuickItem* contentItem = button->property("contentItem").value<QQuickItem*>();
    Q_ASSERT(contentItem);
    return contentItem;
}

ButtonManager::ButtonManager(QObject* parent) : ViewManager(parent), d_ptr(new ButtonManagerPrivate) {}

ButtonManager::~ButtonManager() {}

ViewManager* ButtonManager::viewManager() {
    return this;
}

QString ButtonManager::moduleName() {
    return "RCTButtonViewManager";
}

QStringList ButtonManager::customDirectEventTypes() {
    return QStringList{
        normalizeInputEventName(EVENT_ONPRESSED),
    };
}

QStringList ButtonManager::customBubblingEventTypes() {
    return QStringList{};
}

void ButtonManager::sendPressedNotificationToJs(QQuickItem* button) {
    notifyJsAboutEvent(tag(button), EVENT_ONPRESSED, {});
}

QString ButtonManager::qmlComponentFile(const QVariantMap& properties) const {
    return "qrc:/qml/ReactButton.qml";
}

void ButtonManager::configureView(QQuickItem* button) const {
    ViewManager::configureView(button);
    button->setProperty("buttonManager", QVariant::fromValue((QObject*)this));
}

#include "buttonmanager.moc"
