
#include <QMap>
#include <QCryptographicHash>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkDiskCache>
#include <QQuickImageProvider>

#include "reactimageloader.h"
#include "reactbridge.h"


class ReactImageLoaderPrivate : public QQuickImageProvider {
public:
  ReactImageLoaderPrivate():QQuickImageProvider(QQuickImageProvider::Image)
  {}
  void markCached(const QUrl& source) {
    cacheIds.insert(source, QCryptographicHash::hash(source.toEncoded(), QCryptographicHash::Sha1));
  }
  bool isCached(const QUrl& source) {
    return cacheIds.contains(source);
  }
  QUrl cachedUrl(const QUrl& source) {
    return QUrl("image://react/" + cacheIds.value(source));
  }
  QImage requestImage(const QString& id,  QSize* size, const QSize& requestedSize) {
    qDebug() << __PRETTY_FUNCTION__ << id;
    auto cache = qobject_cast<QNetworkDiskCache*>(bridge->networkAccessManager()->cache());
    auto idev = cache->data(cacheIds.key(id.toLocal8Bit()));
    if (idev == nullptr) {
      qWarning() << __PRETTY_FUNCTION__ << "Could not obtain cache entry for" << id;
      return QImage();
    }
    idev->deleteLater();
    QImage image = QImage::fromData(idev->readAll());
    if (size != nullptr)
      *size = image.size();
    qDebug() << __PRETTY_FUNCTION__ << image;;
    return image;
  }
  QMap<QUrl, QByteArray> cacheIds;
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
      d->markCached(url);
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
  Q_D(ReactImageLoader);
  d->bridge = bridge;
  d->bridge->qmlEngine()->addImageProvider("react", d);
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
  Q_D(ReactImageLoader);
  if (d->isCached(source))
    return d->cachedUrl(source);

  return source;
}
