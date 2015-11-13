
#include <QObject>
#include <QMetaObject>

#include "reactmoduleinterface.h"
#include "reactmoduledata.h"
#include "reactmodulemethod.h"


int ReactModuleData::m_nextModuleId = 1;

namespace
{
// TODO: sort out all the issues around methodsToExport

QList<ReactModuleMethod*> buildMethodList(QObject* moduleImpl)
{
  const QMetaObject* metaObject = moduleImpl->metaObject();
  const int methodCount = metaObject->methodCount();

  QList<ReactModuleMethod*> methods;
  for (int i = metaObject->methodOffset(); i < methodCount; ++i) {
    methods << new ReactModuleMethod(moduleImpl, metaObject->method(i));
  }

  return methods;
}

}

ReactModuleData::ReactModuleData(QObject* moduleImpl)
  : m_id(m_nextModuleId++)
  , m_moduleImpl(moduleImpl)
{
  m_methods = buildMethodList(moduleImpl);
}

ReactModuleData::~ReactModuleData()
{
}

int ReactModuleData::id() const
{
  return m_id;
}

QString ReactModuleData::name() const
{
  return qobject_cast<ReactModuleInterface*>(m_moduleImpl)->moduleName();
}

QVariant ReactModuleData::info() const
{
  ReactModuleInterface* rmi = qobject_cast<ReactModuleInterface*>(m_moduleImpl);

  QVariantMap config;
  config.insert("moduleID", m_id);

  QVariantMap constants = rmi->constantsToExport();
  if (!constants.isEmpty())
    config.insert("constants", constants);

  QVariantMap methodConfig;
  for (int i = 0; i < m_methods.size(); ++i) {
    // TODO: method type
    //    @"type": method.functionType == RCTFunctionTypePromise ? @"remoteAsync" : @"remote",
    methodConfig.insert(m_methods.at(i)->name(), QVariantMap{ {"methodID", i} });
  }
  config.insert("methods", methodConfig);

  return config;
}


ReactModuleMethod* ReactModuleData::method(int id) const
{
  // assert bounds
  return m_methods.at(id);
}

UbuntuViewManager* ReactModuleData::viewManager() const
{
  return qobject_cast<ReactModuleInterface*>(m_moduleImpl)->viewManager();
}
