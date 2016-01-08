
#include <QMetaObject>
#include <QMetaMethod>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include "reactnetworking.h"
#include "reactbridge.h"

ReactNetworking::ReactNetworking(QObject* parent)
  : QObject(parent)
  , m_id(0)
  , m_nam(nullptr)
{
}

ReactNetworking::~ReactNetworking()
{
}

void ReactNetworking::setBridge(ReactBridge* bridge)
{
  m_bridge = bridge;
  m_nam = bridge->networkAccessManager();
}

ReactViewManager* ReactNetworking::viewManager()
{
  return nullptr;
}

QString ReactNetworking::moduleName()
{
  return "RCTNetworking";
}

QList<ReactModuleMethod*> ReactNetworking::methodsToExport()
{
  return QList<ReactModuleMethod*>{};
}

QVariantMap ReactNetworking::constantsToExport()
{
  return QVariantMap{};
}

int ReactNetworking::sendRequest(const QVariantMap& query)
{
  QNetworkRequest request(query["url"].toString());

  QVariantMap headers = query["headers"].toMap();
  Q_FOREACH(const QString& key, headers.keys()) {
    request.setRawHeader(key.toLocal8Bit(), headers[key].toString().toLocal8Bit());
  }

  // TODO: data (post etc)
  // TODO: incrementalUpdates

  if (query["method"].toString().compare("get", Qt::CaseInsensitive) == 0) {
    QNetworkReply* reply = m_nam->get(request);
    QObject::connect(reply, &QNetworkReply::finished, [=] {
        QByteArray body = reply->readAll();
        qDebug() << "Recieved network reply" << body;
        m_bridge->enqueueJSCall("RCTDeviceEventEmitter", "emit", QVariantList{QVariantList{"didReceiveNetworkData", body}});
      });
    m_activeConnections[m_id++] = reply;
  }

  return m_id - 1;
}

void ReactNetworking::cancelRequest(int request)
{
  qDebug() << __PRETTY_FUNCTION__ << request;

  QNetworkReply* reply = m_activeConnections[request];
  if (reply == nullptr)
    return;
  reply->abort();
}

void ReactNetworking::requestFinished()
{
}
