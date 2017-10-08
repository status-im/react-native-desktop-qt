
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

#include "layout/flexbox.h"
#include "reactattachedproperties.h"
#include "reactbridge.h"
#include "reactbuttonmanager.h"
#include "reactpropertyhandler.h"
#include "utilities.h"
#include <functional>

using namespace utilities;

extern "C" {
#include "../../../ReactCommon/yoga/yoga/Yoga.h"
}

#include <QQuickItem>

const QString EVENT_ONPRESSED = "onPress";

class ReactButtonManagerPrivate {
public:
    QQuickItem* buttonTextControl(QQuickItem* button) const;
};

QQuickItem* ReactButtonManagerPrivate::buttonTextControl(QQuickItem* button) const {
    Q_ASSERT(button);
    QQuickItem* contentItem = button->property("contentItem").value<QQuickItem*>();
    Q_ASSERT(contentItem);
    return contentItem;
}

ReactButtonManager::ReactButtonManager(QObject* parent)
    : ReactViewManager(parent), d_ptr(new ReactButtonManagerPrivate) {}

ReactButtonManager::~ReactButtonManager() {}

ReactViewManager* ReactButtonManager::viewManager() {
    return this;
}

QString ReactButtonManager::moduleName() {
    return "RCTButtonViewManager";
}

QStringList ReactButtonManager::customDirectEventTypes() {
    return QStringList{
        normalizeInputEventName(EVENT_ONPRESSED),
    };
}

void ReactButtonManager::sendPressedNotificationToJs(QQuickItem* button) {
    notifyJsAboutEvent(tag(button), EVENT_ONPRESSED, {});
}

QString ReactButtonManager::qmlComponentFile() const {
    return ":/qml/ReactButton.qml";
}

void ReactButtonManager::configureView(QQuickItem* button) const {
    ReactViewManager::configureView(button);
    button->setProperty("buttonManager", QVariant::fromValue((QObject*)this));
}

#include "reactbuttonmanager.moc"
