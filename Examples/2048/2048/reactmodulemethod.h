#ifndef REACTMODULEMETHOD_H
#define REACTMODULEMETHOD_H

#include <QPointer>
#include <QMetaMethod>


class ReactBridge;


class ReactModuleMethod
{
public:
  ReactModuleMethod(QObject* moduleImpl, const QMetaMethod& metaMethod);
  ~ReactModuleMethod();

  QString name() const;

  void invokeWithBridge(ReactBridge* bridge, const QVariantList& args);

private:
  QPointer<QObject> m_moduleImpl;
  QMetaMethod m_metaMethod;
};

#endif // REACTMODULEMETHOD_H
