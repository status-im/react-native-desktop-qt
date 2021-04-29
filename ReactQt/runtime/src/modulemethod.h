
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

#ifndef MODULEMETHOD_H
#define MODULEMETHOD_H

#include <functional>

#include <QMetaMethod>
#include <QPointer>

class Bridge;

enum class NativeMethodType { Async, Promise, Sync };

class ModuleMethod : public QObject {
    Q_OBJECT
public:
    typedef std::function<QObject*(QVariantList&)> ObjectFunction;

    ModuleMethod(const QMetaMethod& metaMethod, const ObjectFunction& objectFunction);
    ~ModuleMethod();

    QString name() const;
    NativeMethodType type() const;

    Q_INVOKABLE void invoke(const QVariantList& args);

private:
    ObjectFunction m_objectFunction;
    QMetaMethod m_metaMethod;
};

#endif // MODULEMETHOD_H
