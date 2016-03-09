#ifndef REACTCOMPONENTDATA_H
#define REACTCOMPONENTDATA_H

#include <QString>
#include <QVariant>

class QQuickItem;
class ReactModuleData;
class ReactViewManager;
class ReactModuleInterface;
class ReactModuleMethod;


class ReactComponentData
{
public:
  ReactComponentData(ReactModuleData* moduleData);
  ~ReactComponentData();

  QString name() const;
  ReactViewManager* manager() const;

  QVariantMap viewConfig() const;

  QQuickItem* createView(int tag, const QVariantMap& properties);

  ReactModuleMethod* method(int id) const;

private:
  ReactModuleData* m_moduleData;
  ReactModuleInterface* m_moduleInterface;
};


#endif // REACTCOMPONENTDATA_H
