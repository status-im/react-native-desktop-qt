
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

#ifndef REACTEVENTDISPATCHER_H
#define REACTEVENTDISPATCHER_H

#include <QObject>


class ReactBridge;


class ReactEventDispatcherPrivate;
class ReactEventDispatcher : public QObject
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(ReactEventDispatcher);

public:
  ReactEventDispatcher(ReactBridge* bridge);
  ~ReactEventDispatcher();

  void sendDeviceEvent(const QString& name, const QVariantList& args);

private:
  QScopedPointer<ReactEventDispatcherPrivate> d_ptr;
};

#endif // REACTEVENTDISPATCHER_H
