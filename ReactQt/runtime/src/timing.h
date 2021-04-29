
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

#ifndef TIMING_H
#define TIMING_H

#include <QMap>
#include <QObject>
#include <QPointer>
#include <QTimer>
#include <QVariant>

#include "moduleinterface.h"

class Bridge;

class Timing : public QObject, public ModuleInterface {
    Q_OBJECT
    // Q_PLUGIN_METADATA(IID ReactModuleInterface_IID)
    Q_INTERFACES(ModuleInterface)

    Q_INVOKABLE void createTimer(int callbackId, int duration, const QDateTime& jsSchedulingTime, bool repeats);
    Q_INVOKABLE void deleteTimer(int timerId);

public:
    Timing(QObject* parent = 0);
    ~Timing();

    void setBridge(Bridge* bridge) override;

    // TODO: this doesnt seem right
    ViewManager* viewManager() override;

    QString moduleName() override;

private:
    void callTimer(const int timerId);

private:
    QPointer<Bridge> m_bridge;
    QMap<int, QTimer*> m_activeTimers;
};

#endif // TIMING_H
