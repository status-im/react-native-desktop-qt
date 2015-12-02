
#include <iterator>
#include <algorithm>

#include <QDateTime>

#include <QDebug>

#include "reactvaluecoercion.h"
#include "reactmoduleinterface.h"
#include "reactbridge.h"

typedef std::function<QVariant (const QVariant&)> coerce_function;

// XXX: should have some way for modules to add these
QMap<int, coerce_function> coerceFunctions
{
  {
    qMetaTypeId<QDateTime>(),
    [](const QVariant& value) {
      Q_ASSERT(value.canConvert<double>());
      return QDateTime::fromMSecsSinceEpoch(value.toLongLong());
    }
  },
  {
    qMetaTypeId<QList<int> >(),
    [](const QVariant& value) {
      Q_ASSERT(value.canConvert<QVariantList>());
      QVariantList s = value.toList();
      QList<int> r;
      std::transform(s.begin(), s.end(), std::back_inserter(r), [](const QVariant& v) { return v.toInt(); });
      return QVariant::fromValue(r);
    }
  },
  {
    qRegisterMetaType<ReactModuleInterface::ResponseBlock>(),
    [](const QVariant& value) {
      Q_ASSERT(value.canConvert(ReactModuleInterface::ResponseBlock));
      int callbackId = value.toInt();
      ReactModuleInterface::ResponseBlock block =
        [callbackId](ReactBridge* bridge, const QVariantList& args) {
          bridge->invokeAndProcess("BatchedBridge",
                                   "invokeCallbackAndReturnFlushedQueue",
                                   QVariantList{ callbackId, args });
        };
      return QVariant::fromValue(block);
    }
  },
  {
    qRegisterMetaType<ReactModuleInterface::ErrorBlock>(),
      [](const QVariant& value) {
      Q_ASSERT(value.canConvert(ReactModuleInterface::ErrorBlock));
      int callbackId = value.toInt();
      ReactModuleInterface::ErrorBlock block =
        [callbackId](ReactBridge* bridge, const QVariantMap& error) {
          bridge->invokeAndProcess("BatchedBridge",
                                   "invokeCallbackAndReturnFlushedQueue",
                                   QVariantList{ callbackId, error });
        };
      return QVariant::fromValue(block);
    }
  }
};


QVariant reactCoerceValue(const QVariant& data, int parameterType)
{
  if (!data.isValid() || data.isNull())
    return QVariant(parameterType, QMetaType::create(parameterType));

  if (data.type() == parameterType)
    return data;

  if (data.canConvert(parameterType)) {
    QVariant converted = data;
    converted.convert(parameterType);
    return converted;
  }

  coerce_function coerceFunction = coerceFunctions.value(parameterType);
  if (!coerceFunction) {
    return QVariant(); // Failure case; an invalid QVariant
  }

  return coerceFunction(data);
}
