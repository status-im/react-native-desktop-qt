
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#ifndef MOUSEEVENTSINTERCEPTOR_H
#define MOUSEEVENTSINTERCEPTOR_H

#include <QScopedPointer>
#include <QString>
#include <QUrl>

#include "reactitem.h"

class Bridge;

class MouseEventsInterceptorPrivate;
class MouseEventsInterceptor : public QQuickItem {
    Q_OBJECT

    Q_DECLARE_PRIVATE(MouseEventsInterceptor)

public:
    MouseEventsInterceptor(QQuickItem* parent = 0);
    ~MouseEventsInterceptor();

    void setBridge(Bridge* bridge);

private:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    bool childMouseEventFilter(QQuickItem* item, QEvent* event) override;
    void sendMouseEvent(QMouseEvent* event, const QString& eventType, QQuickItem* receiver);

    QScopedPointer<MouseEventsInterceptorPrivate> d_ptr;
};

#endif // MOUSEEVENTSINTERCEPTOR_H
