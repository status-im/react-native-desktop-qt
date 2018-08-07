
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

#include "attachedproperties.h"
#include "bridge.h"
#include "layout/flexbox.h"
#include "propertyhandler.h"
#include "slidermanager.h"
#include "utilities.h"

using namespace utilities;

namespace {
const QString EVENT_ON_VALUE_CHANGED = "onValueChange";
const QString EVENT_ON_SLIDING_COMPLETE = "onSlidingComplete";

const char SLIDER_VALUE_PROPERTY_NAME[] = "value";
} // namespace

class SliderManagerPrivate {};

SliderManager::SliderManager(QObject* parent) : ViewManager(parent), d_ptr(new SliderManagerPrivate) {}

SliderManager::~SliderManager() {}

ViewManager* SliderManager::viewManager() {
    return this;
}

QString SliderManager::moduleName() {
    return "RCTSliderManager";
}

QStringList SliderManager::customDirectEventTypes() {
    return QStringList{
        normalizeInputEventName(EVENT_ON_VALUE_CHANGED), normalizeInputEventName(EVENT_ON_SLIDING_COMPLETE),

    };
}

void SliderManager::sendSliderValueChangedToJs(QQuickItem* slider) {
    if (!slider)
        return;
    notifyJsAboutSliderEvent(slider, EVENT_ON_VALUE_CHANGED);
}

void SliderManager::sendSlidingCompleteToJs(QQuickItem* slider) {
    if (!slider)
        return;
    notifyJsAboutSliderEvent(slider, EVENT_ON_SLIDING_COMPLETE);
}

QString SliderManager::qmlComponentFile(const QVariantMap& properties) const {
    return "qrc:/qml/ReactSlider.qml";
}

void SliderManager::configureView(QQuickItem* view) const {
    ViewManager::configureView(view);
    view->setProperty("sliderManager", QVariant::fromValue((QObject*)this));
}

void SliderManager::notifyJsAboutSliderEvent(QQuickItem* slider, const QString& eventName) const {
    if (!slider)
        return;

    double sliderValue = slider->property(SLIDER_VALUE_PROPERTY_NAME).toDouble();
    notifyJsAboutEvent(tag(slider), eventName, QVariantMap{{"target", tag(slider)}, {"value", sliderValue}});
}

#include "slidermanager.moc"
