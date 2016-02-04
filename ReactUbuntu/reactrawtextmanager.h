#ifndef REACTRAWTEXTMANAGER_H
#define REACTRAWTEXTMANAGER_H

#include <QVariant>

#include "reactviewmanager.h"

class QQuickItem;
class ReactPropertyHandler;
// #define QT_STATICPLUGIN

class ReactRawTextManager : public ReactViewManager
{
  Q_OBJECT
  // Q_PLUGIN_METADATA(IID ReactModuleInterface_IID)
  Q_INTERFACES(ReactModuleInterface)

public:
  ReactRawTextManager(QObject *parent = 0);
  ~ReactRawTextManager();

  void setBridge(ReactBridge* bridge) override;

  // TODO: this doesnt seem right
  ReactViewManager* viewManager() override;
  ReactPropertyHandler* propertyHandler(QObject* object) override;

  QString moduleName() override;
  QList<ReactModuleMethod*> methodsToExport() override;
  QVariantMap constantsToExport() override;

  bool shouldLayout() const override;

  QQuickItem* view(const QVariantMap& properties = QVariantMap()) const override;

protected:
  ReactBridge* m_bridge;
};

#endif // UBUNTURAWTEXTMANAGER_H
