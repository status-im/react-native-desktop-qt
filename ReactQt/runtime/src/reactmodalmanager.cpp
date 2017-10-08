
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

#include "layout/flexbox.h"
#include "reactattachedproperties.h"
#include "reactbridge.h"
#include "reactimageloader.h"
#include "reactmodalmanager.h"
#include "reactpropertyhandler.h"
#include "utilities.h"

using namespace utilities;

const QString EVENT_ONSHOW = "onShow";

class ReactModalManagerPrivate {

public:
};

ReactModalManager::ReactModalManager(QObject* parent) : ReactViewManager(parent), d_ptr(new ReactModalManagerPrivate) {}

ReactModalManager::~ReactModalManager() {}

ReactViewManager* ReactModalManager::viewManager() {
    return this;
}

QString ReactModalManager::moduleName() {
    return "RCTModalHostViewManager";
}

void ReactModalManager::addChildItem(QQuickItem* modalView, QQuickItem* child, int position) const {
    // add to parents content item
    QQuickItem* contentItem = QQmlProperty(modalView, "contentItem").read().value<QQuickItem*>();
    Q_ASSERT(contentItem != nullptr);
    child->setParentItem(contentItem);
}

QStringList ReactModalManager::customDirectEventTypes() {
    return QStringList{
        normalizeInputEventName(EVENT_ONSHOW),
    };
}

void ReactModalManager::sendOnShowNotificationToJs(QQuickItem* modal) {
    notifyJsAboutEvent(tag(modal), EVENT_ONSHOW, {});
}

void ReactModalManager::configureView(QQuickItem* modal) const {
    ReactViewManager::configureView(modal);
    modal->setProperty("modalManager", QVariant::fromValue((QObject*)this));
}

QString ReactModalManager::qmlComponentFile() const {
    return ":/qml/ReactModal.qml";
}

#include "reactmodalmanager.moc"
