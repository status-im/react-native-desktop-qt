
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

#ifndef REACTTIMING_H
#define REACTTIMING_H

#include <QMap>
#include <QVariant>
#include <QObject>
#include <QPointer>
#include <QTimer>

#include "reactmoduleinterface.h"

class ReactBridge;

class ReactTiming
  : public QObject
  , public ReactModuleInterface
{
  Q_OBJECT
  // Q_PLUGIN_METADATA(IID ReactModuleInterface_IID)
  Q_INTERFACES(ReactModuleInterface)

  Q_INVOKABLE void createTimer(int callbackId, int duration, const QDateTime& jsSchedulingTime, bool repeats);
  Q_INVOKABLE void deleteTimer(int timerId);

public:
  ReactTiming(QObject* parent = 0);
  ~ReactTiming();

  void setBridge(ReactBridge* bridge) override;

  // TODO: this doesnt seem right
  ReactViewManager* viewManager() override;

  QString moduleName() override;
  QList<ReactModuleMethod*> methodsToExport() override;
  QVariantMap constantsToExport() override;

private:
  QPointer<ReactBridge> m_bridge;
  QMap<int, QTimer*> m_activeTimers;
};

#endif // REACTTIMING_H
