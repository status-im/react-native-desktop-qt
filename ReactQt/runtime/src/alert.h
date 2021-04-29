
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#ifndef ALERT_H
#define ALERT_H

#include "moduleinterface.h"

class AlertPrivate;
class Alert : public QObject, public ModuleInterface {
    Q_OBJECT
    Q_INTERFACES(ModuleInterface)
    Q_DECLARE_PRIVATE(Alert)

    Q_INVOKABLE void alert(const QVariantMap& config, double callback);

public slots:
    void sendButtonClickToJs(double callback, const QString& buttonName);
    void sendDismissedToJs(double callback);

public:
    Alert(QObject* parent = 0);
    virtual ~Alert();

    void setBridge(Bridge* bridge) override;

    QString moduleName() override;

private:
    QScopedPointer<AlertPrivate> d_ptr;
};

#endif // ALERT_H
