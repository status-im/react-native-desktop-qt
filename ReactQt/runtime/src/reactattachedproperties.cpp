
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

#include "reactattachedproperties.h"
#include "reactpropertyhandler.h"
#include "reactviewmanager.h"

class ReactAttachedPropertiesPrivate {
public:
    ReactAttachedPropertiesPrivate() {}
    int tag = -1;
    bool shouldLayout;
    ReactViewManager* viewManager = nullptr;
    ReactPropertyHandler* propertyHandler;
    QQuickItem* item;
};

ReactAttachedProperties::ReactAttachedProperties(QObject* parent)
    : QObject(parent), d_ptr(new ReactAttachedPropertiesPrivate) {
    Q_D(ReactAttachedProperties);
    d->item = qobject_cast<QQuickItem*>(parent);
    if (d->item == nullptr) {
        qCritical() << "ReactAttachedProperties is for use with visual items";
    }
}

ReactAttachedProperties::~ReactAttachedProperties() {}

int ReactAttachedProperties::tag() const {
    return d_func()->tag;
}

void ReactAttachedProperties::setTag(int tag) {
    Q_D(ReactAttachedProperties);
    if (d->tag == tag)
        return;
    d->tag = tag;
}

bool ReactAttachedProperties::shouldLayout() const {
    return d_func()->shouldLayout;
}

void ReactAttachedProperties::setShouldLayout(bool shouldLayout) {
    Q_D(ReactAttachedProperties);
    if (d->shouldLayout == shouldLayout)
        return;
    d->shouldLayout = shouldLayout;
}

ReactViewManager* ReactAttachedProperties::viewManager() const {
    return d_func()->viewManager;
}

void ReactAttachedProperties::setViewManager(ReactViewManager* viewManager) {
    Q_D(ReactAttachedProperties);
    if (d->viewManager == viewManager)
        return;
    d->viewManager = viewManager;
}

ReactPropertyHandler* ReactAttachedProperties::propertyHandler() const {
    return d_func()->propertyHandler;
}

void ReactAttachedProperties::setPropertyHandler(ReactPropertyHandler* propertyHandler) {
    Q_D(ReactAttachedProperties);
    if (d->propertyHandler == propertyHandler)
        return;
    d->propertyHandler = propertyHandler;
}

void ReactAttachedProperties::applyProperties(const QVariantMap& properties) const {
    Q_D(const ReactAttachedProperties);
    Q_ASSERT(d->propertyHandler != nullptr);
    d->propertyHandler->applyProperties(properties);
}

ReactAttachedProperties* ReactAttachedProperties::get(QQuickItem* item, bool create) {
    return qobject_cast<ReactAttachedProperties*>(qmlAttachedPropertiesObject<ReactAttachedProperties>(item, create));
}

ReactAttachedProperties* ReactAttachedProperties::qmlAttachedProperties(QObject* object) {
    return new ReactAttachedProperties(object);
}
