
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

#include <QVariant>

#include "bridge.h"
#include "eventdispatcher.h"

class EventDispatcherPrivate {
public:
    Bridge* bridge;
};

EventDispatcher::EventDispatcher(Bridge* bridge) : QObject(bridge), d_ptr(new EventDispatcherPrivate) {
    Q_D(EventDispatcher);
    d->bridge = bridge;
}

EventDispatcher::~EventDispatcher() {}

void EventDispatcher::sendDeviceEvent(const QString& name, const QVariantList& args) {
    d_func()->bridge->enqueueJSCall("RCTDeviceEventEmitter", "emit", QVariantList{name, args});
}

void EventDispatcher::sendDeviceEvent(const QString& name, const QVariantMap& args) {
    d_func()->bridge->enqueueJSCall("RCTDeviceEventEmitter", "emit", QVariantList{name, args});
}
