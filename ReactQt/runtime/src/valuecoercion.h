
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

#ifndef REACTVALUECOERCION_H
#define REACTVALUECOERCION_H

#include <functional>

#include <QVariant>

typedef std::function<QVariant(const QVariant&)> coerce_function;
typedef QMap<int, coerce_function> coerce_map;

QVariant reactCoerceValue(const QVariant& data, int parameterType, const coerce_map* userCoercions = nullptr);

#endif // REACTVALUECOERCION_H
