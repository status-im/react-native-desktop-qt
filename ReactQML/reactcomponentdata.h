#ifndef REACTCOMPONENTDATA_H
#define REACTCOMPONENTDATA_H

#include <QString>
#include <QVariant>

class QQuickItem;
class UbuntuViewManager;

class ReactComponentData
{
public:
  ReactComponentData(UbuntuViewManager* viewManager);
  ~ReactComponentData();

  QString name() const;
  UbuntuViewManager* manager() const;

  QVariantMap viewConfig() const;

  QQuickItem* createView(int tag, const QVariantMap& properties);

private:
  UbuntuViewManager* m_viewManager;
};


#endif // REACTCOMPONENTDATA_H
