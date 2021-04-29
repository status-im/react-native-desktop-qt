
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

#include "moduledata.h"
#include "moduleinterface.h"
#include "modulemethod.h"

namespace {
// TODO: sort out all the issues around methodsToExport

QList<ModuleMethod*> buildMethodList(QObject* moduleImpl) {
    const QMetaObject* metaObject = moduleImpl->metaObject();
    const int methodCount = metaObject->methodCount();

    QList<ModuleMethod*> methods;

    // from methodsToExport
    ModuleInterface* rmi = qobject_cast<ModuleInterface*>(moduleImpl);
    methods = rmi->methodsToExport();

    for (int i = metaObject->methodOffset(); i < methodCount; ++i) {
        QMetaMethod m = metaObject->method(i);
        if (m.methodType() == QMetaMethod::Method)
            methods << new ModuleMethod(metaObject->method(i), [moduleImpl](QVariantList&) { return moduleImpl; });
    }

    return methods;
}

QVariantMap buildConstantMap(QObject* moduleImpl) {
    const QMetaObject* metaObject = moduleImpl->metaObject();
    const int propertyCount = metaObject->propertyCount();

    QVariantMap constants;

    // from constantsToExport
    ModuleInterface* rmi = qobject_cast<ModuleInterface*>(moduleImpl);
    constants = rmi->constantsToExport();

    // CONSTANT properties (limited usage?) - overrides values from constantsToExport
    for (int i = metaObject->propertyOffset(); i < propertyCount; ++i) {
        QMetaProperty p = metaObject->property(i);
        if (p.isConstant())
            constants.insert(p.name(), p.read(moduleImpl));
    }

    return constants;
}
} // namespace

class ModuleDataPrivate {
public:
    int id;
    QObject* moduleImpl;
    QVariantMap constants;
    QList<ModuleMethod*> methods;
};

ModuleData::ModuleData(QObject* moduleImpl, int id) : d_ptr(new ModuleDataPrivate) {
    Q_D(ModuleData);
    d->id = id;
    d->moduleImpl = moduleImpl;
    d->constants = buildConstantMap(moduleImpl);
    d->methods = buildMethodList(moduleImpl);
}

ModuleData::~ModuleData() {
    Q_D(ModuleData);
    d->moduleImpl->deleteLater();
}

int ModuleData::id() const {
    return d_func()->id;
}

QString ModuleData::name() const {
    return qobject_cast<ModuleInterface*>(d_func()->moduleImpl)->moduleName();
}

QVariant ModuleData::info() const {
    Q_D(const ModuleData);

    QVariantList config;
    QVariantList allMethodsList;
    QVariantList promiseMethodsList;
    QVariantList syncMethodsList;

    for (int i = 0; i < d->methods.size(); ++i) {
        auto method = d->methods.at(i);
        allMethodsList.push_back(method->name());

        if (method->type() == NativeMethodType::Promise) {
            promiseMethodsList.push_back(i);
        }
        if (method->type() == NativeMethodType::Sync) {
            syncMethodsList.push_back(i);
        }
    }

    // TODO: renaming of modules is required, since RCT prefix is not supported by react-native
    config.push_back(name().replace("RCT", ""));
    config.push_back(d->constants.isEmpty() ? QVariant() : d->constants);
    config.push_back(allMethodsList);
    config.push_back(promiseMethodsList);
    config.push_back(syncMethodsList);

    return config;
}

ModuleMethod* ModuleData::method(int id) const {
    // assert bounds
    return d_func()->methods.value(id);
}

ViewManager* ModuleData::viewManager() const {
    return qobject_cast<ModuleInterface*>(d_func()->moduleImpl)->viewManager();
}
