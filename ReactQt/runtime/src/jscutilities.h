
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#ifndef JSCUTILITIES
#define JSCUTILITIES

#include <QVariant>

#include <folly/json.h>

class QQuickItem;
class QQmlEngine;

namespace utilities {

folly::dynamic qvariantToDynamic(const QVariant& value);
QVariant dynamicToQVariant(const folly::dynamic& value);

} // namespace utilities

#endif // JSCUTILITIES
