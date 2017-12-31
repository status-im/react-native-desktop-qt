
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

#ifndef EVENTDISPATCHER_H
#define EVENTDISPATCHER_H

#include <QObject>

class Bridge;

class EventDispatcherPrivate;
class EventDispatcher : public QObject {
    Q_OBJECT
    Q_DECLARE_PRIVATE(EventDispatcher);

public:
    EventDispatcher(Bridge* bridge);
    ~EventDispatcher();

    void sendDeviceEvent(const QString& name, const QVariantList& args);
    void sendDeviceEvent(const QString& name, const QVariantMap& args);

private:
    QScopedPointer<EventDispatcherPrivate> d_ptr;
};

#endif // EVENTDISPATCHER_H
