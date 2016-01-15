#ifndef REACTIMAGEMANAGER_H
#define REACTIMAGEMANAGER_H

#include "reactviewmanager.h"

// #define QT_STATICPLUGIN

class ReactImageManager : public ReactViewManager
{
  Q_OBJECT
  Q_INTERFACES(ReactModuleInterface)

public:
  ReactImageManager(QObject* parent = 0);
  ~ReactImageManager();

  void setBridge(ReactBridge* bridge) override;

  ReactViewManager* viewManager() override;

  QString moduleName() override;
  QList<ReactModuleMethod*> methodsToExport() override;
  QVariantMap constantsToExport() override;

  QQuickItem* view(const QVariantMap& properties) const override;

  void applyProperties(QQuickItem* item, const QVariantMap& properties) const override;
};

#endif // REACTIMAGEMANAGER_H
