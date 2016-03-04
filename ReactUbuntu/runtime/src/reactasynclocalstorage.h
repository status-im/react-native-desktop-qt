#ifndef REACTASYNCLOCALSTORAGE_H
#define REACTASYNCLOCALSTORAGE_H

#include <QByteArray>
#include <QUrl>

#include "reactmoduleinterface.h"


class QSettings;


class ReactAsyncLocalStorage
  : public QObject
  , public ReactModuleInterface
{
  Q_OBJECT
  // Q_PLUGIN_METADATA(IID ReactModuleInterface_IID)
  Q_INTERFACES(ReactModuleInterface)

  Q_INVOKABLE void multiGet(
      const QList<QString>& keys,
      const ReactModuleInterface::ListArgumentBlock& callback);
  Q_INVOKABLE void multiSet(
      const QList<QList<QString>>& kvPairs,
      const ReactModuleInterface::ListArgumentBlock& callback);
  Q_INVOKABLE void multiRemove(
      const QList<QString>& keys,
      const ReactModuleInterface::ListArgumentBlock& callback);
  Q_INVOKABLE void clear(
      const ReactModuleInterface::ListArgumentBlock& callback);
  Q_INVOKABLE void getAllKeys(
      const ReactModuleInterface::ListArgumentBlock& callback);

public:
  ReactAsyncLocalStorage(QObject* parent = 0);
  ~ReactAsyncLocalStorage();

  void setBridge(ReactBridge* bridge) override;

  // TODO: this doesnt seem right
  ReactViewManager* viewManager() override;

  QString moduleName() override;
  QList<ReactModuleMethod*> methodsToExport() override;
  QVariantMap constantsToExport() override;

private:
  ReactBridge* m_bridge;
  QSettings* m_settings;
};

#endif // REACTASYNCLOCALSTORAGE_H
