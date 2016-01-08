#ifndef REACTTEXTMANAGER_H
#define REACTTEXTMANAGER_H

#include "reactrawtextmanager.h"

// #define QT_STATICPLUGIN

class ReactTextManager : public ReactRawTextManager
{
  Q_OBJECT
  // Q_PLUGIN_METADATA(IID ReactModuleInterface_IID)
  Q_INTERFACES(ReactModuleInterface)

public:
  ReactTextManager(QObject* parent = 0);
  ~ReactTextManager();

  void setBridge(ReactBridge* bridge) override;

  ReactViewManager* viewManager() override;

  QString moduleName() override;
  QList<ReactModuleMethod*> methodsToExport() override;
  QVariantMap constantsToExport() override;

  QQuickItem* view(const QVariantMap& properties) const override;

  void applyProperties(QQuickItem* item, const QVariantMap& properties) const override;
};

#endif // REACTTEXTMANAGER_H
