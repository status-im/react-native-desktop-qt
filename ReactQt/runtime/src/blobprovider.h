/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#ifndef BLOBPROVIDER_H
#define BLOBPROVIDER_H

#include "moduleinterface.h"

#include <QVariantMap>

class BlobProviderPrivate;
class BlobProvider : public QObject, public ModuleInterface {
    Q_OBJECT
    Q_INTERFACES(ModuleInterface)

    Q_DECLARE_PRIVATE(BlobProvider)

public:
    BlobProvider(QObject* parent = 0);
    ~BlobProvider();

    void setBridge(Bridge* bridge) override;

    QString moduleName() override;

    Q_INVOKABLE void enableBlobSupport(qlonglong socketId);
    Q_INVOKABLE void disableBlobSupport(qlonglong socketId);
    Q_INVOKABLE void addNetworkingHandler();

private:
    QScopedPointer<BlobProviderPrivate> d_ptr;
};

#endif // BLOBPROVIDER_H
