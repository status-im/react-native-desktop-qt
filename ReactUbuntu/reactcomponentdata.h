#ifndef REACTCOMPONENTDATA_H
#define REACTCOMPONENTDATA_H

#include <QString>
#include <QVariant>

class QQuickItem;
class ReactViewManager;

class ReactComponentData
{
public:
  ReactComponentData(ReactViewManager* viewManager);
  ~ReactComponentData();

  QString name() const;
  ReactViewManager* manager() const;

  QVariantMap viewConfig() const;

  QQuickItem* createView(int tag, const QVariantMap& properties);

private:
  ReactViewManager* m_viewManager;
};


#endif // REACTCOMPONENTDATA_H
