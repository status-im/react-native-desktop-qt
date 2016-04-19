#ifndef REACTVALUECOERCION_H
#define REACTVALUECOERCION_H

#include <functional>

#include <QVariant>


typedef std::function<QVariant (const QVariant&)> coerce_function;
typedef QMap<int, coerce_function> coerce_map;

QVariant reactCoerceValue(const QVariant& data, int parameterType, const coerce_map* userCoercions = nullptr);

#endif // REACTVALUECOERCION_H


