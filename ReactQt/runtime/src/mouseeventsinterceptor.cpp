
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include <QGuiApplication>
#include <QQmlEngine>
#include <QScreen>
#include <QTimer>
#include <QVariant>

#include "attachedproperties.h"
#include "bridge.h"
#include "eventdispatcher.h"
#include "layout/flexbox.h"
#include "mouseeventsinterceptor.h"
#include "uimanager.h"
#include "utilities.h"

using namespace utilities;

class MouseEventsInterceptorPrivate : public QObject {
    Q_OBJECT
    Q_DECLARE_PUBLIC(MouseEventsInterceptor)
public:
    Bridge* bridge = nullptr;
    MouseEventsInterceptor* q_ptr;

    MouseEventsInterceptorPrivate(MouseEventsInterceptor* q) : q_ptr(q) {}
};

MouseEventsInterceptor::MouseEventsInterceptor(QQuickItem* parent)
    : QQuickItem(parent), d_ptr(new MouseEventsInterceptorPrivate(this)) {

    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
    setFiltersChildMouseEvents(true);
}

MouseEventsInterceptor::~MouseEventsInterceptor() {}

void MouseEventsInterceptor::setBridge(Bridge* bridge) {
    d_ptr->bridge = bridge;
}

void MouseEventsInterceptor::mousePressEvent(QMouseEvent* event) {
    sendMouseEvent(event, TOUCH_START, this);
}

// TODO: optimize this
void MouseEventsInterceptor::mouseMoveEvent(QMouseEvent* event) {
    sendMouseEvent(event, TOUCH_MOVE, this);
}

void MouseEventsInterceptor::mouseReleaseEvent(QMouseEvent* event) {
    sendMouseEvent(event, TOUCH_END, this);
}

bool MouseEventsInterceptor::childMouseEventFilter(QQuickItem* item, QEvent* event) {

    switch (event->type()) {
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
    case QEvent::MouseMove:
        break;
    default:
        return false;
    }

    QString eventName;
    if (event->type() == QEvent::MouseButtonPress) {
        eventName = TOUCH_START;
    } else if (event->type() == QEvent::MouseButtonRelease) {
        eventName = TOUCH_END;
    } else if (event->type() == QEvent::MouseMove) {
        eventName = TOUCH_MOVE;
    }

    sendMouseEvent(static_cast<QMouseEvent*>(event), eventName, item);

    return false;
}

void MouseEventsInterceptor::sendMouseEvent(QMouseEvent* event, const QString& eventType, QQuickItem* receiver) {
    Q_D(MouseEventsInterceptor);
    Q_ASSERT(d->bridge);

    QVariantMap e = makeReactTouchEvent(receiver, event);
    if (e.isEmpty())
        return;

    d->bridge->enqueueJSCall("RCTEventEmitter",
                             "receiveTouches",
                             QVariantList{normalizeInputEventName(eventType), QVariantList{e}, QVariantList{0}});
    event->setAccepted(true);
}

#include "mouseeventsinterceptor.moc"
