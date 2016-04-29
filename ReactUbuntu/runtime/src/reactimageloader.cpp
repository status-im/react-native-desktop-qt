
#include <QNetworkRequest>
#include <QNetworkReply>

#include "reactimageloader.h"
#include "reactbridge.h"


class ReactImageLoaderPrivate {
public:
  ReactBridge* bridge = nullptr;
};


void ReactImageLoader::prefetchImage(
  const QString& url,
  const ReactModuleInterface::ListArgumentBlock& resolve,
  const ReactModuleInterface::ListArgumentBlock& reject
) {
  Q_D(ReactImageLoader);

  QNetworkRequest request(url);
  request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferNetwork);
  QNetworkReply* reply = d->bridge->networkAccessManager()->get(request);
  connect(reply, &QNetworkReply::finished, [=]() {
      reply->deleteLater();
      if (reply->error() != QNetworkReply::NoError) {
        reject(d->bridge, QVariantList{QVariantMap{{"code", reply->errorString()}}});
        return;
      }
      resolve(d->bridge, QVariantList{true});
    });
}


ReactImageLoader::ReactImageLoader(QObject* parent)
  : QObject(parent)
  , d_ptr(new ReactImageLoaderPrivate)
{
}

ReactImageLoader::~ReactImageLoader()
{
}

void ReactImageLoader::setBridge(ReactBridge* bridge)
{
  d_func()->bridge = bridge;
}

QString ReactImageLoader::moduleName()
{
  return "RCTImageLoader";
}

QList<ReactModuleMethod*> ReactImageLoader::methodsToExport()
{
  return QList<ReactModuleMethod*>{};
}

QVariantMap ReactImageLoader::constantsToExport()
{
  return QVariantMap{};
}

QUrl ReactImageLoader::provideUriFromSourceUrl(const QUrl& source)
{
  return source;
}
