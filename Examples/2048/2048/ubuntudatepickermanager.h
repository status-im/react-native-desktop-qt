#ifndef UBUNTUDATEPICKERMANAGER_H
#define UBUNTUDATEPICKERMANAGER_H

#include "ubuntuviewmanager.h"

// #define QT_STATICPLUGIN

class UbuntuDatePickerManager : public UbuntuViewManager
{
  Q_OBJECT
  // Q_PLUGIN_METADATA(IID ReactModuleInterface_IID)
  Q_INTERFACES(ReactModuleInterface)

public:
  UbuntuDatePickerManager(QObject* parent = 0);
  ~UbuntuDatePickerManager();

  void setBridge(ReactBridge* bridge);

  UbuntuViewManager* viewManager();

  QString moduleName();
  QStringList methodsToExport();
  QVariantMap constantsToExport();

  QQuickItem* view(const QVariantMap& properties) const;
};

#endif // UBUNTUDATEPICKERMANAGER_H
