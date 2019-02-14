
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

#include <algorithm>
#include <iterator>

#include <QColor>
#include <QDateTime>
#include <QPointF>
#include <QString>

#include <QDebug>

#include "bridge.h"
#include "moduleinterface.h"
#include "networking.h"
#include "valuecoercion.h"

// XXX: should have some way for modules to add these
QMap<int, coerce_function> coerceFunctions{
    {qMetaTypeId<QDateTime>(),
     [](const QVariant& value) {
         Q_ASSERT(value.canConvert<double>());
         return QDateTime::fromMSecsSinceEpoch(value.toLongLong());
     }},
    {qMetaTypeId<QList<int>>(),
     [](const QVariant& value) {
         Q_ASSERT(value.canConvert<QVariantList>());
         QVariantList s = value.toList();
         QList<int> r;
         std::transform(s.begin(), s.end(), std::back_inserter(r), [](const QVariant& v) { return v.toInt(); });
         return QVariant::fromValue(r);
     }},
    {qMetaTypeId<QVector<float>>(),
     [](const QVariant& value) {
         Q_ASSERT(value.canConvert<QVariantList>());
         QVariantList s = value.toList();
         QVector<float> r;
         std::transform(s.begin(), s.end(), std::back_inserter(r), [](const QVariant& v) { return v.toFloat(); });
         return QVariant::fromValue(r);
     }},
    {qMetaTypeId<QList<QString>>(),
     [](const QVariant& value) {
         Q_ASSERT(value.canConvert<QVariantList>());
         QVariantList s = value.toList();
         QList<QString> r;
         std::transform(s.begin(), s.end(), std::back_inserter(r), [](const QVariant& v) { return v.toString(); });
         return QVariant::fromValue(r);
     }},
    {qMetaTypeId<QList<QVariantMap>>(),
     [](const QVariant& value) {
         Q_ASSERT(value.canConvert<QVariantList>());
         QVariantList s = value.toList();
         QList<QVariantMap> r;
         std::transform(s.begin(), s.end(), std::back_inserter(r), [](const QVariant& v) { return v.toMap(); });
         return QVariant::fromValue(r);
     }},
    {qMetaTypeId<QList<QList<QString>>>(),
     [](const QVariant& value) {
         Q_ASSERT(value.canConvert<QVariantList>());
         QVariantList s = value.toList();
         QList<QList<QString>> r;
         std::transform(s.begin(), s.end(), std::back_inserter(r), [](const QVariant& v) { return v.toStringList(); });
         return QVariant::fromValue(r);
     }},
    {qRegisterMetaType<ModuleInterface::ListArgumentBlock>(),
     [](const QVariant& value) {
         Q_ASSERT(value.canConvert<int>());
         int callbackId = value.toInt();
         ModuleInterface::ResponseBlock block = [callbackId](Bridge* bridge, const QVariantList& args) {
             bridge->invokeAndProcess("invokeCallbackAndReturnFlushedQueue", QVariantList{callbackId, args});
         };
         return QVariant::fromValue(block);
     }},
    {qRegisterMetaType<ModuleInterface::MapArgumentBlock>(),
     [](const QVariant& value) {
         Q_ASSERT(value.canConvert<int>());
         int callbackId = value.toInt();
         ModuleInterface::ErrorBlock block = [callbackId](Bridge* bridge, const QVariantMap& error) {
             bridge->invokeAndProcess("invokeCallbackAndReturnFlushedQueue", QVariantList{callbackId, error});
         };
         return QVariant::fromValue(block);
     }},
    {qMetaTypeId<QPointF>(),
     [](const QVariant& value) {
         Q_ASSERT(value.canConvert<QVariantList>());
         QVariantList s = value.toList();
         Q_ASSERT(s.size() == 2);
         return QVariant::fromValue(QPointF(s[0].toDouble(), s[1].toDouble()));
     }},
    {qMetaTypeId<QColor>(), [](const QVariant& value) {
         QColor res;
         if (value.type() == QMetaType::QString) {
             res.setNamedColor(value.toString());
         } else if (value.canConvert<uint>()) {
             res = QColor::fromRgba(value.toUInt());
         }
         return QVariant::fromValue(res);
     }}};

QVariant reactCoerceValue(const QVariant& data, int parameterType, const coerce_map* userCoercions) {
    if (!data.isValid() || data.isNull()) {
        return QVariant(parameterType, QMetaType::create(parameterType));
    }

    if (data.type() == parameterType || parameterType == QMetaType::QVariant) {
        return data;
    }

    coerce_function coerceFunction;

    // User supplied coercions first
    if (userCoercions != nullptr)
        coerceFunction = userCoercions->value(parameterType);

    // RN coercion functions
    if (!coerceFunction)
        coerceFunction = coerceFunctions.value(parameterType);

    if (coerceFunction)
        return coerceFunction(data);

    // QVariant coercions
    if (data.canConvert(parameterType)) {
        QVariant converted = data;
        converted.convert(parameterType);
        return converted;
    }

    return QVariant(); // Failure case; an invalid QVariant
}
