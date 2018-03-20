
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

#include <QMatrix4x4>
#include <QQmlProperty>
#include <QQuickItem>
#include <QString>
#include <QVariant>

#include "attachedproperties.h"
#include "bridge.h"
#include "layout/flexbox.h"
#include "propertyhandler.h"
#include "reactitem.h"
#include "textmanager.h"
#include "utilities.h"
#include "valuecoercion.h"
#include "viewmanager.h"

using namespace utilities;

namespace {
const QString EVENT_ONLAYOUT = "onLayout";
}

ViewManager::ViewManager(QObject* parent) : QObject(parent) {}

ViewManager::~ViewManager() {}

void ViewManager::setBridge(Bridge* bridge) {
    m_bridge = bridge;
}

// TODO: this doesnt seem right
ViewManager* ViewManager::viewManager() {
    return this;
}

PropertyHandler* ViewManager::propertyHandler(QObject* object) {
    return new PropertyHandler(object);
}

QString ViewManager::moduleName() {
    return "RCTViewManager";
}

QStringList ViewManager::customDirectEventTypes() {
    return QStringList{EVENT_ONLAYOUT};
}

QStringList ViewManager::customBubblingEventTypes() {
    return QStringList{"press",
                       "change",
                       "focus",
                       "blur",
                       "submitEditing",
                       "endEditing",
                       "touchStart",
                       "touchMove",
                       "touchCancel",
                       "touchEnd"};
}

bool ViewManager::shouldLayout() const {
    return true;
}

void ViewManager::addChildItem(QQuickItem* container, QQuickItem* child, int position) const {
    child->setParentItem(container);
}

QQuickItem* ViewManager::view(const QVariantMap& properties) const {
    QQuickItem* newView = createView();
    if (newView) {
        configureView(newView);
    }
    return newView;
}

void ViewManager::configureView(QQuickItem* view) const {
    view->setProperty("viewManager", QVariant::fromValue((QObject*)this));
}

QString ViewManager::qmlComponentFile() const {
    return "qrc:/qml/ReactView.qml";
}

void ViewManager::notifyJsAboutEvent(int senderTag, const QString& eventName, const QVariantMap& eventData) const {
    bridge()->enqueueJSCall(
        "RCTEventEmitter", "receiveEvent", QVariantList{senderTag, normalizeInputEventName(eventName), eventData});
}

int ViewManager::tag(QQuickItem* view) {
    AttachedProperties* rap = AttachedProperties::get(view, false);
    if (rap == nullptr) {
        qCritical() << "Could not get reacTag for view!";
        return -1;
    }
    Q_ASSERT(rap);
    return rap->tag();
}

void ViewManager::sendOnLayoutToJs(QQuickItem* view, float x, float y, float width, float height) {
    if (!view)
        return;

    notifyJsAboutEvent(tag(view),
                       EVENT_ONLAYOUT,
                       QVariantMap{{"layout", QVariantMap{{"x", x}, {"y", y}, {"width", width}, {"height", height}}}});
}

QQuickItem* ViewManager::createView() const {
    QQuickItem* item = createQMLItemFromSourceFile(m_bridge->qmlEngine(), QUrl(qmlComponentFile()));
    if (item == nullptr) {
        qCritical() << QString("Can't create QML item for componenet %1").arg(qmlComponentFile());
    }
    return item;
}

Bridge* ViewManager::bridge() const {
    Q_ASSERT(m_bridge);
    return m_bridge;
}

#include "viewmanager.moc"
