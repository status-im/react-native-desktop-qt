
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

#include <QMetaObject>
#include <QObject>

#include <QDebug>

#include "reactmoduledata.h"
#include "reactmoduleinterface.h"
#include "reactmodulemethod.h"

namespace
{
// TODO: sort out all the issues around methodsToExport

QList<ReactModuleMethod*> buildMethodList(QObject* moduleImpl) {
    const QMetaObject* metaObject = moduleImpl->metaObject();
    const int methodCount = metaObject->methodCount();

    QList<ReactModuleMethod*> methods;

    // from methodsToExport
    ReactModuleInterface* rmi = qobject_cast<ReactModuleInterface*>(moduleImpl);
    methods = rmi->methodsToExport();

    for (int i = metaObject->methodOffset(); i < methodCount; ++i) {
        QMetaMethod m = metaObject->method(i);
        if (m.methodType() == QMetaMethod::Method)
            methods << new ReactModuleMethod(metaObject->method(i), [moduleImpl](QVariantList&) { return moduleImpl; });
    }

    return methods;
}

QVariantMap buildConstantMap(QObject* moduleImpl) {
    const QMetaObject* metaObject = moduleImpl->metaObject();
    const int propertyCount = metaObject->propertyCount();

    QVariantMap constants;

    // from constantsToExport
    ReactModuleInterface* rmi = qobject_cast<ReactModuleInterface*>(moduleImpl);
    constants = rmi->constantsToExport();

    // CONSTANT properties (limited usage?) - overrides values from constantsToExport
    for (int i = metaObject->propertyOffset(); i < propertyCount; ++i) {
        QMetaProperty p = metaObject->property(i);
        if (p.isConstant())
            constants.insert(p.name(), p.read(moduleImpl));
    }

    return constants;
}
}

class ReactModuleDataPrivate {
public:
    int id;
    QObject* moduleImpl;
    QVariantMap constants;
    QList<ReactModuleMethod*> methods;
};

ReactModuleData::ReactModuleData(QObject* moduleImpl, int id)
    : d_ptr(new ReactModuleDataPrivate)
{
    Q_D(ReactModuleData);
    d->id = id;
    d->moduleImpl = moduleImpl;
    d->constants = buildConstantMap(moduleImpl);
    d->methods = buildMethodList(moduleImpl);
}

ReactModuleData::~ReactModuleData() {
    Q_D(ReactModuleData);
    d->moduleImpl->deleteLater();
}

int ReactModuleData::id() const {
    return d_func()->id;
}

QString ReactModuleData::name() const {
    return qobject_cast<ReactModuleInterface*>(d_func()->moduleImpl)->moduleName();
}

QVariant ReactModuleData::info() const {
    Q_D(const ReactModuleData);

    QVariantList config;
    QVariantList asyncMethodsList;
    QVariantList promiseMethodsList;
    QVariantList syncMethodsList;

    for (int i = 0; i < d->methods.size(); ++i) {
        auto method = d->methods.at(i);
        asyncMethodsList.push_back(method->name());

        if (method->type() == NativeMethodType::Promise) {
            promiseMethodsList.push_back(i);
        }
    }

    // TODO: renaming of modules is required, since RCT prefix is not supported by react-native
    config.push_back(name().replace("RCT", ""));
    config.push_back(d->constants.isEmpty() ? QVariant() : d->constants);
    config.push_back(asyncMethodsList);
    config.push_back(promiseMethodsList);
    config.push_back(syncMethodsList);

    return config;
}

ReactModuleMethod* ReactModuleData::method(int id) const {
    // assert bounds
    return d_func()->methods.value(id);
}

ReactViewManager* ReactModuleData::viewManager() const {
    return qobject_cast<ReactModuleInterface*>(d_func()->moduleImpl)->viewManager();
}
