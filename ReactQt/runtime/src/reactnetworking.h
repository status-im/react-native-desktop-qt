
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

#ifndef REACTNETWORKING_H
#define REACTNETWORKING_H

#include <functional>

#include <QObject>
#include <QVariant>

#include "reactmoduleinterface.h"

class ReactNetworkingPrivate;
class ReactNetworking : public QObject, public ReactModuleInterface {
    Q_OBJECT
    // Q_PLUGIN_METADATA(IID ReactModuleInterface_IID)
    Q_INTERFACES(ReactModuleInterface)

    Q_INVOKABLE void sendRequest(
        int requestId, const QString& method, const QUrl& url, const QVariantMap& headers, const QByteArray& data);
    Q_INVOKABLE void abortRequest(int requestId);

    Q_DECLARE_PRIVATE(ReactNetworking);

public:
    ReactNetworking(QObject* parent = 0);
    ~ReactNetworking();

    void setBridge(ReactBridge* bridge) override;

    // TODO: this doesnt seem right
    ReactViewManager* viewManager() override;

    QString moduleName() override;
    QList<ReactModuleMethod*> methodsToExport() override;
    QVariantMap constantsToExport() override;

private:
    QScopedPointer<ReactNetworkingPrivate> d_ptr;
};

#endif // REACTNETWORKING_H
