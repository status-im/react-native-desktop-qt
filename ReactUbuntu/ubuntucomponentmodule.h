#ifndef UBUNTUCOMPONENTMODULE_H
#define UBUNTUCOMPONENTMODULE_H

#include <QObject>

#include "reactviewmanager.h"


class UbuntuComponentModule : public ReactViewManager
{
  Q_OBJECT

public:
  UbuntuComponentModule(const QString& name, const QString& version, const QString& path);
  ~UbuntuComponentModule();

  void setBridge(ReactBridge* bridge);

  ReactViewManager* viewManager() override;

  QString moduleName() override;
  QList<ReactModuleMethod*> methodsToExport() override;
  QVariantMap constantsToExport() override;

  QQuickItem* view(const QVariantMap& properties) const override;

  void applyProperties(QQuickItem* item, const QVariantMap& properties) const override;

private:
  QString m_name;
  QString m_version;
  QString m_path;
  bool m_methodsExported;
  QList<ReactModuleMethod*> m_methodCache;
};

#endif // UBUNTUCOMPONENTMODULE_H
