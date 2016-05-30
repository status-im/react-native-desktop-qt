
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

#include "reacteventdispatcher.h"
#include "reactbridge.h"


class ReactEventDispatcherPrivate {
public:
  ReactBridge* bridge;
};


ReactEventDispatcher::ReactEventDispatcher(ReactBridge* bridge)
  : QObject(bridge)
  , d_ptr(new ReactEventDispatcherPrivate)
{
  Q_D(ReactEventDispatcher);
  d->bridge = bridge;
}

ReactEventDispatcher::~ReactEventDispatcher()
{
}

void ReactEventDispatcher::sendDeviceEvent(const QString& name, const QVariantList& args)
{
  d_func()->bridge->enqueueJSCall("RCTDeviceEventEmitter",
                                  "emit",
                                  QVariantList{name, args});
}
