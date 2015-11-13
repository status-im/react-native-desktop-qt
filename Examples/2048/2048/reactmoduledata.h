#ifndef REACTMODULEDATA_H
#define REACTMODULEDATA_H

#include <QList>

class QObject;

class ReactModuleInteface;
class ReactModuleMethod;
class UbuntuViewManager;


class ReactModuleData
{
public:
  ReactModuleData(QObject* moduleImpl);
  ~ReactModuleData();

  int id() const;
  QString name() const;

  QVariant info() const;

  ReactModuleMethod* method(int id) const;

  UbuntuViewManager* viewManager() const;

private:
  int m_id;
  QObject* m_moduleImpl;
  QList<ReactModuleMethod*> m_methods;

  static int m_nextModuleId;
};

#endif // REACTMODULEDATA_H
