
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

#ifndef NETWORKING_H
#define NETWORKING_H

#include <functional>

#include <QLoggingCategory>
#include <QObject>
#include <QVariant>

#include "moduleinterface.h"

Q_DECLARE_LOGGING_CATEGORY(NETWORKING)

class NetworkingPrivate;
class Networking : public QObject, public ModuleInterface {
    Q_OBJECT
    // Q_PLUGIN_METADATA(IID ReactModuleInterface_IID)
    Q_INTERFACES(ModuleInterface)

    Q_INVOKABLE void sendRequest(const QString& method,
                                 const QUrl& url,
                                 const QVariantMap& data,
                                 const QVariantMap& headers,
                                 const QString& responseType,
                                 bool incrementalUpdates,
                                 qlonglong timeout,
                                 bool withCredentials,
                                 double requestIdCallbackId);
    Q_INVOKABLE void abortRequest(int requestId);

    Q_DECLARE_PRIVATE(Networking);

public:
    Networking(QObject* parent = 0);
    ~Networking();

    void setBridge(Bridge* bridge) override;

    // TODO: this doesnt seem right
    ViewManager* viewManager() override;

    QString moduleName() override;

private:
    QScopedPointer<NetworkingPrivate> d_ptr;
};

#endif // NETWORKING_H
