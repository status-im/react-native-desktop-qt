
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

#ifndef REACTMODULEDATA_H
#define REACTMODULEDATA_H

#include <QScopedPointer>

class QObject;
class ReactModuleMethod;
class ReactViewManager;

class ReactModuleDataPrivate;
class ReactModuleData {
    Q_DECLARE_PRIVATE(ReactModuleData)

public:
    ReactModuleData(QObject* moduleImpl, int id);
    ~ReactModuleData();

    int id() const;
    QString name() const;

    QVariant info() const;

    ReactModuleMethod* method(int id) const;

    ReactViewManager* viewManager() const;

private:
    QScopedPointer<ReactModuleDataPrivate> d_ptr;
};

#endif // REACTMODULEDATA_H
