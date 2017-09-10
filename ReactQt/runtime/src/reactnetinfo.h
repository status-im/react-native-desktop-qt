
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

#ifndef REACTNETINFO_H
#define REACTNETINFO_H

#include "reactmoduleinterface.h"

class ReactNetInfoPrivate;
class ReactNetInfo : public QObject, public ReactModuleInterface {
    Q_OBJECT
    Q_INTERFACES(ReactModuleInterface)

    Q_INVOKABLE REACT_PROMISE void getCurrentConnectivity(const ReactModuleInterface::ListArgumentBlock& resolve,
                                                          const ReactModuleInterface::ListArgumentBlock& reject);

    Q_DECLARE_PRIVATE(ReactNetInfo)

public:
    ReactNetInfo(QObject* parent = 0);
    ~ReactNetInfo();

    void setBridge(ReactBridge* bridge) override;

    QString moduleName() override;
    QList<ReactModuleMethod*> methodsToExport() override;
    QVariantMap constantsToExport() override;

private:
    QScopedPointer<ReactNetInfoPrivate> d_ptr;
};

#endif // REACTNETINFO_H
