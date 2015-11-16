#ifndef REACTVALUECOERCION_H
#define REACTVALUECOERCION_H

#include <functional>

#include <QVariant>


typedef std::function<QVariant (const QVariant&)> coerce_function;

QVariant reactCoerceValue(const QVariant& data, int parameterType);

#endif // REACTVALUECOERCION_H


