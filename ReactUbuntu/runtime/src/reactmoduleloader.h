
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

#ifndef REACTMODULELOADER_H
#define REACTMODULELOADER_H

#include <QObject>

class ReactModuleLoader
{
public:
  virtual ~ReactModuleLoader() {}

  virtual QObjectList availableModules() = 0;
};

#define ReactModuleLoader_IID "com.canonical.ReactNative.ModuleLoader"

Q_DECLARE_INTERFACE(ReactModuleLoader, ReactModuleLoader_IID);

#endif // REACTMODULELOADER_H
