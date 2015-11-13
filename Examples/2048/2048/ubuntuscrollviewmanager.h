#ifndef UBUNTUSCROLLVIEWMANAGER_H
#define UBUNTUSCROLLVIEWMANAGER_H

#include "ubuntuviewmanager.h"

// #define QT_STATICPLUGIN

class UbuntuScrollViewManager : public UbuntuViewManager
{
  Q_OBJECT
  // Q_PLUGIN_METADATA(IID ReactModuleInterface_IID)
  Q_INTERFACES(ReactModuleInterface)

public:
  UbuntuScrollViewManager(QObject* parent = 0);
  ~UbuntuScrollViewManager();

  void setBridge(ReactBridge* bridge);

  UbuntuViewManager* viewManager();

  QString moduleName();
  QStringList methodsToExport();
  QVariantMap constantsToExport();

  QQuickItem* view(const QVariantMap& properties) const;
};

#endif // UBUNTUSCROLLVIEWMANAGER_H
