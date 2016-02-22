#ifndef REACTCOMPONENTDATA_H
#define REACTCOMPONENTDATA_H

#include <QString>
#include <QVariant>

class QQuickItem;
class ReactModuleInterface;
class ReactViewManager;
class ReactPropertyHandler;


class ReactComponentData
{
public:
  ReactComponentData(ReactModuleInterface* moduleInteface);
  ~ReactComponentData();

  QString name() const;
  ReactViewManager* manager() const;

  QVariantMap viewConfig() const;

  QQuickItem* createView(int tag, const QVariantMap& properties);

private:
  ReactModuleInterface* m_moduleInterface;
};


#endif // REACTCOMPONENTDATA_H
