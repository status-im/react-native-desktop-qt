#ifndef REACTVIEWMANAGER_H
#define REACTVIEWMANAGER_H

#include <QVariant>

#include "reactmoduleinterface.h"

class QQuickItem;

// #define QT_STATICPLUGIN

class ReactViewManager
  : public QObject
  , public ReactModuleInterface
{
  Q_OBJECT
  // Q_PLUGIN_METADATA(IID ReactModuleInterface_IID)
  Q_INTERFACES(ReactModuleInterface)

public:
  ReactViewManager(QObject *parent = 0);
  ~ReactViewManager();

  void setBridge(ReactBridge* bridge) override;

  // TODO: this doesnt seem right
  ReactViewManager* viewManager() override;

  QString moduleName() override;
  QList<ReactModuleMethod*> methodsToExport() override;
  QVariantMap constantsToExport() override;

  virtual QStringList customDirectEventTypes();
  virtual QStringList customBubblingEventTypes();

  virtual QQuickItem* view(const QVariantMap& properties = QVariantMap()) const;

  virtual void applyProperties(QQuickItem* item, const QVariantMap& properties) const;

protected:
  ReactBridge* m_bridge;
};

#endif // REACTVIEWMANAGER_H
