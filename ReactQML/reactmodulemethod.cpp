
#include <QVector>

#include <QDebug>

#include "reactmodulemethod.h"
#include "reactvaluecoercion.h"


ReactModuleMethod::ReactModuleMethod(QObject* moduleImpl, const QMetaMethod& metaMethod)
  : m_moduleImpl(moduleImpl)
  , m_metaMethod(metaMethod)
{
}

ReactModuleMethod::~ReactModuleMethod()
{
}

QString ReactModuleMethod::name() const
{
  return m_metaMethod.name();
}

// meh
#define _R_ARG(argn) QGenericArgument(argn.typeName(), argn.data())

// TODO: don't use direct connection
void ReactModuleMethod::invokeWithBridge(ReactBridge* bridge, const QVariantList& args)
{
  //qDebug() << __PRETTY_FUNCTION__ << "module" << m_moduleImpl << "name" << m_metaMethod.methodSignature();
  const int parameterCount = m_metaMethod.parameterCount();

  if (args.size() != parameterCount) {
    qCritical() << "Attempt to invoke" << m_metaMethod.methodSignature() <<
      "with" << args.size() << "arguments";
    return;
  }

  QVariantList pa;
  for (int i = 0; i < parameterCount; ++i) {
    pa << reactCoerceValue(args.at(i), m_metaMethod.parameterType(i));
    if (!pa.last().isValid()) {
      qCritical() << "Could not convert argument" << i << "for" <<
        m_metaMethod.methodSignature() << "from" << args.at(i).typeName();
      return;
    }
  }

  // come on, why after all this time, is there no dynamic invoke?
  switch (args.size()) {
  case 0:
    m_metaMethod.invoke(m_moduleImpl, Qt::DirectConnection);
    break;
  case 1:
    m_metaMethod.invoke(m_moduleImpl, Qt::DirectConnection, _R_ARG(pa.at(0)));
    break;
  case 2:
    m_metaMethod.invoke(m_moduleImpl, Qt::DirectConnection,
                        _R_ARG(pa.at(0)), _R_ARG(pa.at(1)));
    break;
  case 3:
    m_metaMethod.invoke(m_moduleImpl, Qt::DirectConnection,
                        _R_ARG(pa.at(0)), _R_ARG(pa.at(1)), _R_ARG(pa.at(2)));
    break;
  case 4:
    m_metaMethod.invoke(m_moduleImpl, Qt::DirectConnection,
                        _R_ARG(pa.at(0)), _R_ARG(pa.at(1)), _R_ARG(pa.at(2)), _R_ARG(pa.at(3)));
    break;
  case 5:
    m_metaMethod.invoke(m_moduleImpl, Qt::DirectConnection,
                        _R_ARG(pa.at(0)), _R_ARG(pa.at(1)), _R_ARG(pa.at(2)), _R_ARG(pa.at(3)), _R_ARG(pa.at(4)));
    break;
  case 6:
    m_metaMethod.invoke(m_moduleImpl, Qt::DirectConnection,
                        _R_ARG(pa.at(0)), _R_ARG(pa.at(1)), _R_ARG(pa.at(2)), _R_ARG(pa.at(3)), _R_ARG(pa.at(4)),
                        _R_ARG(pa.at(5)));
    break;
  case 7:
    m_metaMethod.invoke(m_moduleImpl, Qt::DirectConnection,
                        _R_ARG(pa.at(0)), _R_ARG(pa.at(1)), _R_ARG(pa.at(2)), _R_ARG(pa.at(3)), _R_ARG(pa.at(4)),
                        _R_ARG(pa.at(5)), _R_ARG(pa.at(6)));
  case 8:
    m_metaMethod.invoke(m_moduleImpl, Qt::DirectConnection,
                        _R_ARG(pa.at(0)), _R_ARG(pa.at(1)), _R_ARG(pa.at(2)), _R_ARG(pa.at(3)), _R_ARG(pa.at(4)),
                        _R_ARG(pa.at(5)), _R_ARG(pa.at(6)), _R_ARG(pa.at(7)));
    break;
  }
}
