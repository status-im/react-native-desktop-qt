
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

#include <QQmlComponent>
#include <QQmlProperty>
#include <QQuickItem>
#include <QString>
#include <QVariant>

#include <QDebug>

#include "attachedproperties.h"
#include "bridge.h"
#include "imageloader.h"
#include "layout/flexbox.h"
#include "modalmanager.h"
#include "mouseeventsinterceptor.h"
#include "propertyhandler.h"
#include "utilities.h"

using namespace utilities;

const QString EVENT_ONSHOW = "onShow";

class ModalManagerPrivate {

public:
};

ModalManager::ModalManager(QObject* parent) : ViewManager(parent), d_ptr(new ModalManagerPrivate) {}

ModalManager::~ModalManager() {}

ViewManager* ModalManager::viewManager() {
    return this;
}

QString ModalManager::moduleName() {
    return "RCTModalHostViewManager";
}

void ModalManager::addChildItem(QQuickItem* modalView, QQuickItem* child, int position) const {
    // add to parents content item
    QQuickItem* contentItem = QQmlProperty(modalView, "contentItem").read().value<QQuickItem*>();
    Q_ASSERT(contentItem != nullptr);
    utilities::insertChildItemAt(child, position, contentItem);
}

QStringList ModalManager::customDirectEventTypes() {
    return QStringList{
        normalizeInputEventName(EVENT_ONSHOW),
    };
}

void ModalManager::sendOnShowNotificationToJs(QQuickItem* modal) {
    notifyJsAboutEvent(tag(modal), EVENT_ONSHOW, {});
}

void ModalManager::configureView(QQuickItem* modal) const {
    ViewManager::configureView(modal);
    modal->setProperty("modalManager", QVariant::fromValue((QObject*)this));
    MouseEventsInterceptor* contentItem = QQmlProperty(modal, "contentItem").read().value<MouseEventsInterceptor*>();
    contentItem->setBridge(bridge());
}

QString ModalManager::qmlComponentFile(const QVariantMap& properties) const {
    return "qrc:/qml/ReactModal.qml";
}

#include "modalmanager.moc"
