#ifndef REACTIMAGELOADER_H
#define REACTIMAGELOADER_H

#include <QUrl>

#include "reactmoduleinterface.h"


class ReactImageLoaderPrivate;
class ReactImageLoader
  : public QObject
  , public ReactModuleInterface
{
  Q_OBJECT
  // Q_PLUGIN_METADATA(IID ReactModuleInterface_IID)
  Q_INTERFACES(ReactModuleInterface)

  Q_INVOKABLE REACT_PROMISE
    void prefetchImage(const QString& url,
                      const ReactModuleInterface::ListArgumentBlock& resolve,
                      const ReactModuleInterface::ListArgumentBlock& reject);

  Q_DECLARE_PRIVATE(ReactImageLoader)

public:
  ReactImageLoader(QObject* parent = 0);
  ~ReactImageLoader();

  void setBridge(ReactBridge* bridge) override;

  QString moduleName() override;
  QList<ReactModuleMethod*> methodsToExport() override;
  QVariantMap constantsToExport() override;

  QUrl provideUriFromSourceUrl(const QUrl& source);

private:
  QScopedPointer<ReactImageLoaderPrivate> d_ptr;
};

#endif // REACTIMAGELOADER_H
