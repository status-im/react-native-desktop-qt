#ifndef UBUNTUACTIVITYMANAGER_H
#define UBUNTUACTIVITYMANAGER_H

#include "reactviewmanager.h"

// #define QT_STATICPLUGIN

class UbuntuActivityManager : public ReactViewManager
{
  Q_OBJECT
  // Q_PLUGIN_METADATA(IID ReactModuleInterface_IID)
  Q_INTERFACES(ReactModuleInterface)

public:
  UbuntuActivityManager(QObject* parent = 0);
  ~UbuntuActivityManager();

  void setBridge(ReactBridge* bridge) override;

  ReactViewManager* viewManager() override;

  QString moduleName() override;
  QStringList methodsToExport() override;
  QVariantMap constantsToExport() override;

  QQuickItem* view(const QVariantMap& properties) const override;
};

#endif // UBUNTUACTIVITYMANAGER_H
