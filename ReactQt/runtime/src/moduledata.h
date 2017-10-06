
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

#ifndef MODULEDATA_H
#define MODULEDATA_H

#include <QScopedPointer>

class QObject;
class ModuleMethod;
class ViewManager;

class ModuleDataPrivate;
class ModuleData : public QObject {
    Q_DECLARE_PRIVATE(ModuleData)
    Q_OBJECT

public:
    ModuleData(QObject* moduleImpl, int id);
    ~ModuleData();

    int id() const;
    QString name() const;

    QVariant info() const;

    ModuleMethod* method(int id) const;

    ViewManager* viewManager() const;

private:
    QScopedPointer<ModuleDataPrivate> d_ptr;
};

#endif // MODULEDATA_H
