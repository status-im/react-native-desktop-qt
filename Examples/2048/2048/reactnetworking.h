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

public:
  ReactNetworking(QObject* parent = 0);
  ~ReactNetworking();

  void setBridge(ReactBridge* bridge);

  // TODO: this doesnt seem right
  UbuntuViewManager* viewManager();

  QString moduleName();
  QStringList methodsToExport();
  QVariantMap constantsToExport();

  Q_INVOKABLE int sendRequest(const QVariantMap& query);
  Q_INVOKABLE void cancelRequest(int request);

private:
  int m_id;
  QNetworkAccessManager* m_nam;
  QMap<int, QNetworkReply*> m_activeConnections;
};

#endif // REACTNETWORKING_H
