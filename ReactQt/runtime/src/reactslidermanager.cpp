
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include <QDebug>
#include <QQmlComponent>
#include <QQmlProperty>
#include <QQuickItem>
#include <QString>
#include <QVariant>

#include "layout/flexbox.h"
#include "reactattachedproperties.h"
#include "reactbridge.h"
#include "reactevents.h"
#include "reactpropertyhandler.h"
#include "reactslidermanager.h"

namespace {
const QString EVENT_ON_VALUE_CHANGED = "onValueChange";
const QString EVENT_ON_SLIDING_COMPLETE = "onSlidingComplete";

const char SLIDER_VALUE_PROPERTY_NAME[] = "value";
}

class ReactSliderManagerPrivate {};

ReactSliderManager::ReactSliderManager(QObject* parent)
    : ReactViewManager(parent), d_ptr(new ReactSliderManagerPrivate) {}

ReactSliderManager::~ReactSliderManager() {}

ReactViewManager* ReactSliderManager::viewManager() {
    return this;
}

QString ReactSliderManager::moduleName() {
    return "RCTSliderManager";
}

QStringList ReactSliderManager::customDirectEventTypes() {
    return QStringList{
        normalizeInputEventName(EVENT_ON_VALUE_CHANGED), normalizeInputEventName(EVENT_ON_SLIDING_COMPLETE),
    };
}

void ReactSliderManager::sendSliderValueChangedToJs(QQuickItem* slider) {
    if (!slider)
        return;
    QVariantList args = sliderValueEventArg(slider, EVENT_ON_VALUE_CHANGED);
    bridge()->enqueueJSCall("RCTEventEmitter", "receiveEvent", args);
}

void ReactSliderManager::sendSlidingCompleteToJs(QQuickItem* slider) {
    if (!slider)
        return;
    QVariantList args = sliderValueEventArg(slider, EVENT_ON_SLIDING_COMPLETE);
    bridge()->enqueueJSCall("RCTEventEmitter", "receiveEvent", args);
}

QString ReactSliderManager::qmlComponentFile() const {
    return ":/qml/ReactSlider.qml";
}

void ReactSliderManager::configureView(QQuickItem* view) const {
    ReactViewManager::configureView(view);
    view->setProperty("sliderManager", QVariant::fromValue((QObject*)this));
}

QVariantList ReactSliderManager::sliderValueEventArg(QQuickItem* slider, const QString& eventName) const {
    if (!slider)
        return QVariantList();

    double sliderValue = slider->property(SLIDER_VALUE_PROPERTY_NAME).toDouble();
    int reactTag = ReactAttachedProperties::get(slider)->tag();
    return QVariantList{
        reactTag, normalizeInputEventName(eventName), QVariantMap{{"target", reactTag}, {"value", sliderValue}}};
}

#include "reactslidermanager.moc"
