
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

#include <QMetaMethod>
#include <QMetaObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include "bridge.h"
#include "timing.h"

Timing::Timing(QObject* parent) : QObject(parent) {}

Timing::~Timing() {}

void Timing::setBridge(Bridge* bridge) {
    m_bridge = bridge;
}

ViewManager* Timing::viewManager() {
    return nullptr;
}

QString Timing::moduleName() {
    return "RCTTiming";
}

void Timing::callTimer(const int timerId) {
    if (m_bridge) {
        QVariantList args;
        QVariantList timersArray{timerId};
        args.push_back(timersArray);
        m_bridge->enqueueJSCall("JSTimers", "callTimers", args);
    }
}

/*
 React Behaviour (objc)
 Reacts timers are based off the screen refresh callback from CADisplayLink.
 Making the resolution of the timer in line with screen refresh. (60hz?)

 It attempts to compensate for request latency, by passing the time the request
 was scheduled.

 - if the duration is 0 and it does not repeat, a callback is enqueud
   immediately, and this method returns
 - if the duration is less than 0.018secs (roughly one frame?) its coerced to 0
   (but not enqueued immediately)
 - a RCTTimer is created with the target time

 Loop:
  - the refresh cycle checks RCTTimers and triggers if they are current||past

 Notes:
  - requestAnimationFrame asks for a callback after duration 1ms, this
    effectively means it will served on the next frame refresh
  - This version; maybe the view, or some part, should be a custom node in the
    scene graph to properly sync (especially re: animations)
*/

void Timing::createTimer(int timerId, int duration /*ms*/, const QDateTime& jsSchedulingTime, bool repeats) {

    if (duration == 0 && !repeats) {
        // XXX: enqueueJSCall should enqueue
        QTimer::singleShot(0, [=] { callTimer(timerId); });
        return;
    }

    QTimer* timer = new QTimer(this);
    timer->setTimerType(Qt::PreciseTimer);
    timer->setSingleShot(!repeats);

    QObject::connect(timer, &QTimer::timeout, this, [=]() {
        callTimer(timerId);
        if (!repeats)
            deleteTimer(timerId);
    });
    m_activeTimers[timerId] = timer;
    if (duration < 18) {
        // XXX: mimicking RN behavior via timeouts
        timer->start((1.0 / 60) * 1000);
    } else {
        timer->start(duration);
    }
}

void Timing::deleteTimer(int timerId) {
    // qDebug() << __PRETTY_FUNCTION__ << timerId;
    QMap<int, QTimer*>::iterator it = m_activeTimers.find(timerId);
    if (it == m_activeTimers.end())
        return;

    (*it)->stop();
    (*it)->deleteLater();
    m_activeTimers.erase(it);
}
