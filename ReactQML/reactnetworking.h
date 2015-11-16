#ifndef REACTNETWORKING_H
#define REACTNETWORKING_H

#include <QObject>
#include <QVariant>

#include "reactmoduleinterface.h"

class QNetworkAccessManager;
class QNetworkReply;


class ReactNetworking
  : public QObject
  , public ReactModuleInterface
{
  Q_OBJECT
  // Q_PLUGIN_METADATA(IID ReactModuleInterface_IID)
  Q_INTERFACES(ReactModuleInterface)

  Q_INVOKABLE int sendRequest(const QVariantMap& query);
  Q_INVOKABLE void cancelRequest(int request);

public:
  ReactNetworking(QObject* parent = 0);
  ~ReactNetworking();

  void setBridge(ReactBridge* bridge) override;

  // TODO: this doesnt seem right
  UbuntuViewManager* viewManager() override;

  QString moduleName() override;
  QStringList methodsToExport() override;
  QVariantMap constantsToExport() override;

private:
  int m_id;
  QNetworkAccessManager* m_nam;
  QMap<int, QNetworkReply*> m_activeConnections;
};

#endif // REACTNETWORKING_H
