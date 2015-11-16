#ifndef UBUNTUVIEWMANAGER_H
#define UBUNTUVIEWMANAGER_H

#include <QVariant>

#include "reactmoduleinterface.h"

class QQuickItem;

// #define QT_STATICPLUGIN

class UbuntuViewManager
  : public QObject
  , public ReactModuleInterface
{
  Q_OBJECT
  // Q_PLUGIN_METADATA(IID ReactModuleInterface_IID)
  Q_INTERFACES(ReactModuleInterface)

public:
  UbuntuViewManager(QObject *parent = 0);
  ~UbuntuViewManager();

  void setBridge(ReactBridge* bridge) override;

  // TODO: this doesnt seem right
  UbuntuViewManager* viewManager() override;

  QString moduleName() override;
  QStringList methodsToExport() override;
  QVariantMap constantsToExport() override;

  virtual QQuickItem* view(const QVariantMap& properties = QVariantMap()) const;

protected:
  ReactBridge* m_bridge;
};

#endif // UBUNTUVIEWMANAGER_H
