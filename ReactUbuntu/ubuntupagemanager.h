#ifndef UBUNTUPAGEMANAGER_H
#define UBUNTUPAGEMANAGER_H

#include <QString>
#include <QMap>

#include "reactviewmanager.h"


class QQuickItem;

class UbuntuPageManager : public ReactViewManager
{
  Q_OBJECT
  Q_INTERFACES(ReactModuleInterface)

public:
  UbuntuPageManager(QObject* parent = 0);
  ~UbuntuPageManager();

  void setBridge(ReactBridge* bridge) override;

  ReactViewManager* viewManager() override;
  ReactPropertyHandler* propertyHandler(QObject* object);

  QString moduleName() override;
  QList<ReactModuleMethod*> methodsToExport() override;
  QVariantMap constantsToExport() override;

  QQuickItem* view(const QVariantMap& properties) const override;

private:
  mutable int m_id;
  QMap<QPair<QString, QQuickItem*>, QMetaMethod> m_methodCache;
};

#endif // UBUNTUPAGEMANAGER_H
