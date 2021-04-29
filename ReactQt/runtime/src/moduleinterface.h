
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

#ifndef MODULEINTERFACE_H
#define MODULEINTERFACE_H

#include <functional>

#include <QVariantMap>
#include <QtPlugin>

class Bridge;
class ViewManager;
class PropertyHandler;
class ModuleMethod;

#ifndef Q_MOC_RUN
#define REACT_PROMISE
#define REACT_SYNC
#endif

class ModuleInterface {
public:
    typedef std::function<void(Bridge*, const QVariantMap&)> MapArgumentBlock;
    typedef std::function<void(Bridge*, const QVariantList&)> ListArgumentBlock;

    // XXX:
    typedef ListArgumentBlock ResponseBlock;
    typedef MapArgumentBlock ErrorBlock;
    typedef MapArgumentBlock BubblingEventBlock;
    typedef MapArgumentBlock DirectEventBlock;

    virtual ~ModuleInterface() {}

    virtual void setBridge(Bridge* bridge) {
        Q_UNUSED(bridge);
    }

    // TODO: this doesnt seem right
    virtual ViewManager* viewManager() {
        return nullptr;
    }
    virtual PropertyHandler* propertyHandler(QObject* object) {
        return nullptr;
    }

    virtual QString moduleName() = 0;

    virtual QList<ModuleMethod*> methodsToExport() {
        return QList<ModuleMethod*>{};
    }

    virtual QVariantMap constantsToExport() {
        return QVariantMap{};
    }
};

#define ModuleInterface_IID "com.canonical.ReactNative.ModuleInterface"

Q_DECLARE_INTERFACE(ModuleInterface, ModuleInterface_IID);

Q_DECLARE_METATYPE(ModuleInterface::MapArgumentBlock);
Q_DECLARE_METATYPE(ModuleInterface::ListArgumentBlock);

#endif // MODULEINTERFACE_H
