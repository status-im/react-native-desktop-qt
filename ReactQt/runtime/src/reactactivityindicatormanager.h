
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#ifndef REACTATCTIVITYINDICATORMANAGER_H
#define REACTATCTIVITYINDICATORMANAGER_H

#include "reactviewmanager.h"

class ReactPropertyHandler;
class ReactActivityIndicatorManagerPrivate;
class ReactActivityIndicatorManager : public ReactViewManager {
    Q_OBJECT
    Q_INTERFACES(ReactModuleInterface)
    Q_DECLARE_PRIVATE(ReactActivityIndicatorManager)

public:
    ReactActivityIndicatorManager(QObject* parent = 0);
    virtual ~ReactActivityIndicatorManager();

    virtual ReactViewManager* viewManager() override;
    virtual QString moduleName() override;

private:
    virtual QString qmlComponentFile() const override;

private:
    QScopedPointer<ReactActivityIndicatorManagerPrivate> d_ptr;
};

#endif // REACTATCTIVITYINDICATORMANAGER_H
