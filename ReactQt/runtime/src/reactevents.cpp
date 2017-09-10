
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

#include "reactevents.h"

QString normalizeInputEventName(const QString& eventName) {
    QString tmp = eventName;
    if (eventName.startsWith("top"))
        return tmp;

    if (eventName.startsWith("on")) {
        tmp.replace(0, 2, "top");
    } else {
        tmp[0] = tmp[0].toUpper();
        tmp = "top" + tmp;
    }

    return tmp;
}
