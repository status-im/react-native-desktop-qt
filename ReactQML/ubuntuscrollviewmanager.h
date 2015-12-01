#ifndef UBUNTUSCROLLVIEWMANAGER_H
#define UBUNTUSCROLLVIEWMANAGER_H

#include "reactviewmanager.h"

// #define QT_STATICPLUGIN

class UbuntuScrollViewManager : public ReactViewManager
{
  Q_OBJECT
  // Q_PLUGIN_METADATA(IID ReactModuleInterface_IID)
  Q_INTERFACES(ReactModuleInterface)

public:
  UbuntuScrollViewManager(QObject* parent = 0);
  ~UbuntuScrollViewManager();

  void setBridge(ReactBridge* bridge) override;

  ReactViewManager* viewManager() override;

  QString moduleName() override;
  QStringList methodsToExport() override;
  QVariantMap constantsToExport() override;

  QQuickItem* view(const QVariantMap& properties) const override;
};

#endif // UBUNTUSCROLLVIEWMANAGER_H
