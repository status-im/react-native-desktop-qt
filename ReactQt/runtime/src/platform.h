/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#ifndef PLATFORM_H
#define PLATFORM_H

#include "moduleinterface.h"

#include <QVariantMap>

class PlatformPrivate;
class Platform : public QObject, public ModuleInterface {
    Q_OBJECT
    Q_INTERFACES(ModuleInterface)

    Q_DECLARE_PRIVATE(Platform)

public:
    Platform(QObject* parent = 0);
    ~Platform();

    void setBridge(Bridge* bridge) override;

    QString moduleName() override;
    QVariantMap constantsToExport() override;

private:
    QScopedPointer<PlatformPrivate> d_ptr;
};

#endif // PLATFORM_H
