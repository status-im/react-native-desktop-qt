
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

#ifndef NETINFO_H
#define NETINFO_H

#include "moduleinterface.h"

class NetInfoPrivate;
class NetInfo : public QObject, public ModuleInterface {
    Q_OBJECT
    Q_INTERFACES(ModuleInterface)

    Q_INVOKABLE REACT_PROMISE void getCurrentConnectivity(const ModuleInterface::ListArgumentBlock& resolve,
                                                          const ModuleInterface::ListArgumentBlock& reject);

    Q_INVOKABLE void setConnectionCheckUrl(const QString& url);

    Q_DECLARE_PRIVATE(NetInfo)

public:
    NetInfo(QObject* parent = 0);
    ~NetInfo();

    void setBridge(Bridge* bridge) override;

    QString moduleName() override;

private:
    QScopedPointer<NetInfoPrivate> d_ptr;
};

#endif // NETINFO_H
