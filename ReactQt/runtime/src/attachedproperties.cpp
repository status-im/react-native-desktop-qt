
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

#include <QQuickItem>

#include "attachedproperties.h"
#include "componentmanagers/viewmanager.h"
#include "propertyhandler.h"

class AttachedPropertiesPrivate {
public:
    AttachedPropertiesPrivate() {}
    int tag = -1;
    bool shouldLayout;
    ViewManager* viewManager = nullptr;
    PropertyHandler* propertyHandler;
    QQuickItem* item;
};

AttachedProperties::AttachedProperties(QObject* parent) : QObject(parent), d_ptr(new AttachedPropertiesPrivate) {
    Q_D(AttachedProperties);
    d->item = qobject_cast<QQuickItem*>(parent);
    if (d->item == nullptr) {
        qCritical() << "AttachedProperties is for use with visual items";
    }
}

AttachedProperties::~AttachedProperties() {}

int AttachedProperties::tag() const {
    return d_func()->tag;
}

void AttachedProperties::setTag(int tag) {
    Q_D(AttachedProperties);
    if (d->tag == tag)
        return;
    d->tag = tag;
}

bool AttachedProperties::shouldLayout() const {
    return d_func()->shouldLayout;
}

void AttachedProperties::setShouldLayout(bool shouldLayout) {
    Q_D(AttachedProperties);
    if (d->shouldLayout == shouldLayout)
        return;
    d->shouldLayout = shouldLayout;
}

ViewManager* AttachedProperties::viewManager() const {
    return d_func()->viewManager;
}

void AttachedProperties::setViewManager(ViewManager* viewManager) {
    Q_D(AttachedProperties);
    if (d->viewManager == viewManager)
        return;
    d->viewManager = viewManager;
}

PropertyHandler* AttachedProperties::propertyHandler() const {
    return d_func()->propertyHandler;
}

void AttachedProperties::setPropertyHandler(PropertyHandler* propertyHandler) {
    Q_D(AttachedProperties);
    if (d->propertyHandler == propertyHandler)
        return;
    d->propertyHandler = propertyHandler;
}

void AttachedProperties::applyProperties(const QVariantMap& properties) const {
    Q_D(const AttachedProperties);
    Q_ASSERT(d->propertyHandler != nullptr);
    d->propertyHandler->applyProperties(properties);
}

AttachedProperties* AttachedProperties::get(QQuickItem* item, bool create) {
    return qobject_cast<AttachedProperties*>(qmlAttachedPropertiesObject<AttachedProperties>(item, create));
}

AttachedProperties* AttachedProperties::qmlAttachedProperties(QObject* object) {
    return new AttachedProperties(object);
}
