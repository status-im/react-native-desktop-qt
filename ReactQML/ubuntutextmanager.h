#ifndef UBUNTUTEXTMANAGER_H
#define UBUNTUTEXTMANAGER_H

#include "ubuntuviewmanager.h"

// #define QT_STATICPLUGIN

class UbuntuTextManager : public UbuntuViewManager
{
  Q_OBJECT
  // Q_PLUGIN_METADATA(IID ReactModuleInterface_IID)
  Q_INTERFACES(ReactModuleInterface)

public:
  UbuntuTextManager(QObject* parent = 0);
  ~UbuntuTextManager();

  void setBridge(ReactBridge* bridge) override;

  UbuntuViewManager* viewManager() override;

  QString moduleName() override;
  QStringList methodsToExport() override;
  QVariantMap constantsToExport() override;

  QQuickItem* view(const QVariantMap& properties) const override;
};

#endif // UBUNTUTEXTMANAGER_H
