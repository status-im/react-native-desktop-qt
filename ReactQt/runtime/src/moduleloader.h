
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

#ifndef MODULELOADER_H
#define MODULELOADER_H

#include <QObject>

class ModuleLoader {
public:
    virtual ~ModuleLoader() {}

    virtual QObjectList availableModules() = 0;
};

#define ModuleLoader_IID "com.canonical.ReactNative.ModuleLoader"

Q_DECLARE_INTERFACE(ModuleLoader, ModuleLoader_IID);

#endif // MODULELOADER_H
