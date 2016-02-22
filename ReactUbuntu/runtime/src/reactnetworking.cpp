
#include <QMetaObject>
#include <QMetaMethod>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include "reactnetworking.h"
#include "reactbridge.h"

ReactNetworking::ReactNetworking(QObject* parent)
  : QObject(parent)
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

void ReactNetworking::sendRequest(
  int requestId,
  const QString& method,
  const QUrl& url,
  const QVariantMap& headers,
  const QByteArray& data,
  const ReactNetworking::Callback& callback
)
{
  qDebug() << __PRETTY_FUNCTION__ << method << url;

  QNetworkRequest request(url);

  for (const QString& key : headers.keys()) {
    request.setRawHeader(key.toLocal8Bit(), headers[key].toString().toLocal8Bit());
  }

  // TODO: data (post etc)

  if (method.compare("get", Qt::CaseInsensitive) == 0) {
    QNetworkReply* reply = m_nam->get(request);
    QObject::connect(reply, &QNetworkReply::finished, [=] {
        callback(m_bridge,
                 reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(),
                 QVariantMap{},
                 reply->readAll());
      });
    m_activeConnections[requestId] = reply;
  }
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
