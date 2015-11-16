
#include <QDateTime>

#include "reactvaluecoercion.h"

// XXX: should have some way for modules to add these
QMap<int, coerce_function> coerceFunctions
{
  {
    qMetaTypeId<QDateTime>(),
    [](const QVariant& value) { return QDateTime::fromMSecsSinceEpoch(value.toLongLong()); }
  }
};

QVariant reactCoerceValue(const QVariant& data, int parameterType)
{
  coerce_function coerceFunction = coerceFunctions.value(parameterType);
  if (!coerceFunction) {
    return QVariant();
  }

  return coerceFunction(data);
}
