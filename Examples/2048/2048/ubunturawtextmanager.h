#ifndef UBUNTURAWTEXTMANAGER_H
#define UBUNTURAWTEXTMANAGER_H

#include <QVariant>

#include "ubuntuviewmanager.h"

class QQuickItem;

// #define QT_STATICPLUGIN

class UbuntuRawTextManager : public UbuntuViewManager
{
  Q_OBJECT
  // Q_PLUGIN_METADATA(IID ReactModuleInterface_IID)
  Q_INTERFACES(ReactModuleInterface)

public:
  UbuntuRawTextManager(QObject *parent = 0);
  ~UbuntuRawTextManager();

  void setBridge(ReactBridge* bridge);

  // TODO: this doesnt seem right
  UbuntuViewManager* viewManager();

  QString moduleName();
  QStringList methodsToExport();
  QVariantMap constantsToExport();

  QQuickItem* view(const QVariantMap& properties = QVariantMap()) const;

protected:
  ReactBridge* m_bridge;
};

#endif // UBUNTURAWTEXTMANAGER_H
