/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#ifndef REACTBLOBPROVIDER_H
#define REACTBLOBPROVIDER_H

#include "reactmoduleinterface.h"

#include <QVariantMap>

class ReactBlobProviderPrivate;
class ReactBlobProvider : public QObject, public ReactModuleInterface {
    Q_OBJECT
    Q_INTERFACES(ReactModuleInterface)

    Q_DECLARE_PRIVATE(ReactBlobProvider)

public:
    ReactBlobProvider(QObject* parent = 0);
    ~ReactBlobProvider();

    void setBridge(ReactBridge* bridge) override;

    QString moduleName() override;
    QList<ReactModuleMethod*> methodsToExport() override;
    QVariantMap constantsToExport() override;

private:
    QScopedPointer<ReactBlobProviderPrivate> d_ptr;
};

#endif // REACTBLOBPROVIDER_H
