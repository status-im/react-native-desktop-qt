#ifndef REACTRAWTEXTMANAGER_H
#define REACTRAWTEXTMANAGER_H

#include <QVariant>

#include "reactviewmanager.h"

class QQuickItem;

// #define QT_STATICPLUGIN

class ReactRawTextManager : public ReactViewManager
{
  Q_OBJECT
  // Q_PLUGIN_METADATA(IID ReactModuleInterface_IID)
  Q_INTERFACES(ReactModuleInterface)

public:
  ReactRawTextManager(QObject *parent = 0);
  ~ReactRawTextManager();

  void setBridge(ReactBridge* bridge) override;

  // TODO: this doesnt seem right
  ReactViewManager* viewManager() override;

  QString moduleName() override;
  QStringList methodsToExport() override;
  QVariantMap constantsToExport() override;

  QQuickItem* view(const QVariantMap& properties = QVariantMap()) const override;

  void applyProperties(QQuickItem* item, const QVariantMap& properties) const override;

protected:
  ReactBridge* m_bridge;
};

#endif // UBUNTURAWTEXTMANAGER_H
