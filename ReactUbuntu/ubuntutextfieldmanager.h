#ifndef UBUNTUTEXTFIELDMANAGER_H
#define UBUNTUTEXTFIELDMANAGER_H

#include "reactviewmanager.h"


class UbuntuTextFieldManager : public ReactViewManager
{
  Q_OBJECT
  Q_INTERFACES(ReactModuleInterface)

public:
  UbuntuTextFieldManager(QObject* parent = 0);
  ~UbuntuTextFieldManager();

  void setBridge(ReactBridge* bridge) override;

  ReactViewManager* viewManager() override;
  ReactPropertyHandler* propertyHandler(QObject* object) override;

  QString moduleName() override;
  QList<ReactModuleMethod*> methodsToExport() override;
  QVariantMap constantsToExport() override;

  QStringList customDirectEventTypes() override;

  QQuickItem* view(const QVariantMap& properties) const override;

private Q_SLOTS:
  void activeFocusChanged();
  void textChanged();

private:
  void configureView(QQuickItem* view) const;
};

#endif // UBUNTUTEXTFIELDMANAGER_H
