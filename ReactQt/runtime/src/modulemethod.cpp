
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

#include <QVector>

#include <QDebug>

#include "modulemethod.h"
#include "valuecoercion.h"

ModuleMethod::ModuleMethod(const QMetaMethod& metaMethod, const ObjectFunction& objectFunction)
    : m_metaMethod(metaMethod), m_objectFunction(objectFunction) {}

ModuleMethod::~ModuleMethod() {}

QString ModuleMethod::name() const {
    return m_metaMethod.name();
}

NativeMethodType ModuleMethod::type() const {
    if (QString(m_metaMethod.tag()) == "REACT_PROMISE") {
        return NativeMethodType::Promise;
    } else if (QString(m_metaMethod.tag()) == "REACT_SYNC") {
        return NativeMethodType::Sync;
    }

    return NativeMethodType::Async;
}

// meh
#define _R_ARG(argn) QGenericArgument(argn.typeName(), argn.data())

// TODO: don't use direct connection
void ModuleMethod::invoke(const QVariantList& args) {
    QVariantList argsm = args;
    QObject* target = m_objectFunction(argsm);
    if (target == nullptr) {
        qWarning() << "Could not find target for invoking function" << m_metaMethod.methodSignature();
        return;
    }

    // qDebug() << __PRETTY_FUNCTION__ << "module" << target << "name" << m_metaMethod.methodSignature();

    const int parameterCount = m_metaMethod.parameterCount();

    if (argsm.size() != parameterCount) {
        qCritical() << "Attempt to invoke" << m_metaMethod.methodSignature() << "with" << argsm.size() << "arguments";
        return;
    }

    QVariantList pa;
    for (int i = 0; i < parameterCount; ++i) {
        pa << reactCoerceValue(argsm.at(i), m_metaMethod.parameterType(i));
        if (!pa.last().isValid()) {
            qCritical() << "Could not convert argument" << i << "for" << m_metaMethod.methodSignature() << "from"
                        << argsm.at(i).typeName();
            return;
        }
    }

    switch (argsm.size()) {
    case 0:
        m_metaMethod.invoke(target, Qt::DirectConnection);
        break;
    case 1:
        m_metaMethod.invoke(target, Qt::DirectConnection, _R_ARG(pa.at(0)));
        break;
    case 2:
        m_metaMethod.invoke(target, Qt::DirectConnection, _R_ARG(pa.at(0)), _R_ARG(pa.at(1)));
        break;
    case 3:
        m_metaMethod.invoke(target, Qt::DirectConnection, _R_ARG(pa.at(0)), _R_ARG(pa.at(1)), _R_ARG(pa.at(2)));
        break;
    case 4:
        m_metaMethod.invoke(
            target, Qt::DirectConnection, _R_ARG(pa.at(0)), _R_ARG(pa.at(1)), _R_ARG(pa.at(2)), _R_ARG(pa.at(3)));
        break;
    case 5:
        m_metaMethod.invoke(target,
                            Qt::DirectConnection,
                            _R_ARG(pa.at(0)),
                            _R_ARG(pa.at(1)),
                            _R_ARG(pa.at(2)),
                            _R_ARG(pa.at(3)),
                            _R_ARG(pa.at(4)));
        break;
    case 6:
        m_metaMethod.invoke(target,
                            Qt::DirectConnection,
                            _R_ARG(pa.at(0)),
                            _R_ARG(pa.at(1)),
                            _R_ARG(pa.at(2)),
                            _R_ARG(pa.at(3)),
                            _R_ARG(pa.at(4)),
                            _R_ARG(pa.at(5)));
        break;
    case 7:
        m_metaMethod.invoke(target,
                            Qt::DirectConnection,
                            _R_ARG(pa.at(0)),
                            _R_ARG(pa.at(1)),
                            _R_ARG(pa.at(2)),
                            _R_ARG(pa.at(3)),
                            _R_ARG(pa.at(4)),
                            _R_ARG(pa.at(5)),
                            _R_ARG(pa.at(6)));
        break;
    case 8:
        m_metaMethod.invoke(target,
                            Qt::DirectConnection,
                            _R_ARG(pa.at(0)),
                            _R_ARG(pa.at(1)),
                            _R_ARG(pa.at(2)),
                            _R_ARG(pa.at(3)),
                            _R_ARG(pa.at(4)),
                            _R_ARG(pa.at(5)),
                            _R_ARG(pa.at(6)),
                            _R_ARG(pa.at(7)));
        break;
    case 9:
        m_metaMethod.invoke(target,
                            Qt::DirectConnection,
                            _R_ARG(pa.at(0)),
                            _R_ARG(pa.at(1)),
                            _R_ARG(pa.at(2)),
                            _R_ARG(pa.at(3)),
                            _R_ARG(pa.at(4)),
                            _R_ARG(pa.at(5)),
                            _R_ARG(pa.at(6)),
                            _R_ARG(pa.at(7)),
                            _R_ARG(pa.at(8)));
        break;
    }
}
