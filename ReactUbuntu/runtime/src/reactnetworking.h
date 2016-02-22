#ifndef REACTNETWORKING_H
#define REACTNETWORKING_H

#include <functional>

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
  typedef std::function<void (ReactBridge*, int requestId, const QVariantMap& headers, const QByteArray& responseText)> Callback;

private:
  Q_INVOKABLE void sendRequest(int requestId,
                              const QString& method,
                              const QUrl& url,
                              const QVariantMap& headers,
                              const QByteArray& data,
                              const ReactNetworking::Callback& callback);
  Q_INVOKABLE void cancelRequest(int requestId);

public:
  ReactNetworking(QObject* parent = 0);
  ~ReactNetworking();

  void setBridge(ReactBridge* bridge) override;

  // TODO: this doesnt seem right
  ReactViewManager* viewManager() override;

  QString moduleName() override;
  QList<ReactModuleMethod*> methodsToExport() override;
  QVariantMap constantsToExport() override;

private Q_SLOTS:
  void requestFinished();

private:
  ReactBridge* m_bridge;
  QNetworkAccessManager* m_nam;
  QMap<int, QNetworkReply*> m_activeConnections;
};

Q_DECLARE_METATYPE(ReactNetworking::Callback);

#endif // REACTNETWORKING_H
