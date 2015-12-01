#ifndef UBUNTUDATEPICKERMANAGER_H
#define UBUNTUDATEPICKERMANAGER_H

#include "reactviewmanager.h"

// #define QT_STATICPLUGIN

class UbuntuDatePickerManager : public ReactViewManager
{
  Q_OBJECT
  // Q_PLUGIN_METADATA(IID ReactModuleInterface_IID)
  Q_INTERFACES(ReactModuleInterface)

public:
  UbuntuDatePickerManager(QObject* parent = 0);
  ~UbuntuDatePickerManager();

  void setBridge(ReactBridge* bridge) override;

  ReactViewManager* viewManager() override;

  QString moduleName() override;
  QStringList methodsToExport() override;
  QVariantMap constantsToExport() override;

  QQuickItem* view(const QVariantMap& properties) const override;
};

#endif // UBUNTUDATEPICKERMANAGER_H
