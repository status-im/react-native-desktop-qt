#ifndef REACTMODULEMETHOD_H
#define REACTMODULEMETHOD_H

#include <functional>

#include <QPointer>
#include <QMetaMethod>


class ReactBridge;


class ReactModuleMethod
{
public:
  typedef std::function<QObject* (QVariantList&)> ObjectFunction;

  ReactModuleMethod(const QMetaMethod& metaMethod, const ObjectFunction& objectFunction);
  ~ReactModuleMethod();

  QString name() const;

  void invokeWithBridge(ReactBridge* bridge, const QVariantList& args);

private:
  ObjectFunction m_objectFunction;
  QMetaMethod m_metaMethod;
};


#endif // REACTMODULEMETHOD_H
