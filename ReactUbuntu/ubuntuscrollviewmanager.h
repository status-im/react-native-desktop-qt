#ifndef UBUNTUSCROLLVIEWMANAGER_H
#define UBUNTUSCROLLVIEWMANAGER_H

#include "reactviewmanager.h"

// #define QT_STATICPLUGIN

class UbuntuScrollViewManager : public ReactViewManager
{
  Q_OBJECT
  // Q_PLUGIN_METADATA(IID ReactModuleInterface_IID)
  Q_INTERFACES(ReactModuleInterface)

public:
  UbuntuScrollViewManager(QObject* parent = 0);
  ~UbuntuScrollViewManager();

  void setBridge(ReactBridge* bridge) override;

  ReactViewManager* viewManager() override;
  // ReactPropertyHandler* propertyHandler(QObject* object) override;

  QString moduleName() override;
  QList<ReactModuleMethod*> methodsToExport() override;
  QVariantMap constantsToExport() override;

  QStringList customDirectEventTypes() override;

  QQuickItem* view(const QVariantMap& properties) const override;

private Q_SLOTS:
  void scrollBeginDrag();
  void scrollEndDrag();
  void scroll();

  void momentumScrollBegin();
  void momentumScrollEnd();

private:
  void configureView(QQuickItem* view) const;
};

#endif // UBUNTUSCROLLVIEWMANAGER_H
