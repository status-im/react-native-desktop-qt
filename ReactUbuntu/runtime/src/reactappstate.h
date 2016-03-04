#ifndef REACTAPPSTATE_H
#define REACTAPPSTATE_H

#include <QByteArray>
#include <QUrl>

#include "reactmoduleinterface.h"


class ReactAppState
  : public QObject
  , public ReactModuleInterface
{
  Q_OBJECT
  // Q_PLUGIN_METADATA(IID ReactModuleInterface_IID)
  Q_INTERFACES(ReactModuleInterface)

  Q_INVOKABLE void getCurrentAppState(
      const ReactModuleInterface::ListArgumentBlock& success,
      const ReactModuleInterface::MapArgumentBlock& error);

public:
  ReactAppState(QObject* parent = 0);
  ~ReactAppState();

  void setBridge(ReactBridge* bridge) override;

  // TODO: this doesnt seem right
  ReactViewManager* viewManager() override;

  QString moduleName() override;
  QList<ReactModuleMethod*> methodsToExport() override;
  QVariantMap constantsToExport() override;

private:
  ReactBridge* m_bridge;
};

#endif // REACTAPPSTATE_H
