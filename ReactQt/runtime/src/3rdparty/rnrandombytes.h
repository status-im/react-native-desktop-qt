
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

#ifndef RNRANDOMBYTES_H
#define RNRANDOMBYTES_H

#include <QUrl>

#include "moduleinterface.h"

class RNRandomBytesPrivate;
class RNRandomBytes : public QObject, public ModuleInterface {
    Q_OBJECT

    Q_INTERFACES(ModuleInterface)

    Q_DECLARE_PRIVATE(RNRandomBytes)

public:
    enum Event { Event_LoadStart, Event_Progress, Event_LoadError, Event_LoadSuccess, Event_LoadEnd };

    typedef std::function<void(Event, const QVariantMap&)> LoadEventCallback;

    RNRandomBytes(QObject* parent = 0);
    ~RNRandomBytes();

    QString moduleName() override;
    QList<ModuleMethod*> methodsToExport() override;
    QVariantMap constantsToExport() override;


private:
    QScopedPointer<RNRandomBytesPrivate> d_ptr;
};

#endif // RNRANDOMBYTES_H
