
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

#include "reactplugin.h"
#include "utilities.h"

#include <QLoggingCategory>

void ReactPlugin::registerTypes(const char* uri) {
    utilities::registerReactTypes();
    QLoggingCategory::setFilterRules("UIManager=true\n"
                                     "Flexbox=true\n"
                                     "WebSocketModule=true\n"
                                     "Networking=true\n"
                                     "ViewManager=true\n");
}
