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

  void setBridge(ReactBridge* bridge);

  UbuntuViewManager* viewManager();

  QString moduleName();
  QStringList methodsToExport();
  QVariantMap constantsToExport();

  QQuickItem* view(const QVariantMap& properties) const;
};

#endif // UBUNTUTEXTMANAGER_H
