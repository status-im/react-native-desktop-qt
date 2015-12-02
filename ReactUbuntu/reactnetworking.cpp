
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

QStringList ReactNetworking::methodsToExport()
{
  // TODO: move into moduledata
  const QMetaObject* metaObject = this->metaObject();
  const int methodCount = metaObject->methodCount();

  QStringList methods;
  for (int i = metaObject->methodOffset(); i < methodCount; ++i) {
    methods << metaObject->method(i).name();
  }
  return methods;
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

  if (query["method"].toString() == "get") {
    QNetworkReply* reply = m_nam->get(request);
    connect(reply, SIGNAL(finished()), SLOT(requestFinished()));
    m_activeConnections[m_id++] = reply;
  }

  return m_id - 1;
}

void ReactNetworking::cancelRequest(int request)
{
  QNetworkReply* reply = m_activeConnections[request];
  if (reply == nullptr)
    return;
  reply->abort();
}
