#ifndef UBUNTUACTIVITYMANAGER_H
#define UBUNTUACTIVITYMANAGER_H

#include "ubuntuviewmanager.h"

// #define QT_STATICPLUGIN

class UbuntuActivityManager : public UbuntuViewManager
{
  Q_OBJECT
  // Q_PLUGIN_METADATA(IID ReactModuleInterface_IID)
  Q_INTERFACES(ReactModuleInterface)

public:
  UbuntuActivityManager(QObject* parent = 0);
  ~UbuntuActivityManager();

  void setBridge(ReactBridge* bridge);

  UbuntuViewManager* viewManager();

  QString moduleName();
  QStringList methodsToExport();
  QVariantMap constantsToExport();

  QQuickItem* view(const QVariantMap& properties) const;
};

#endif // UBUNTUACTIVITYMANAGER_H
