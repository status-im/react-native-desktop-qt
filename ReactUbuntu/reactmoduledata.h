#ifndef REACTMODULEDATA_H
#define REACTMODULEDATA_H

#include <QScopedPointer>

class QObject;
class ReactModuleMethod;
class ReactViewManager;


class ReactModuleDataPrivate;
class ReactModuleData
{
  Q_DECLARE_PRIVATE(ReactModuleData)

public:
  ReactModuleData(QObject* moduleImpl);
  ~ReactModuleData();

  int id() const;
  QString name() const;

  QVariant info() const;

  ReactModuleMethod* method(int id) const;

  ReactViewManager* viewManager() const;

private:
  QScopedPointer<ReactModuleDataPrivate> d_ptr;
};

#endif // REACTMODULEDATA_H
