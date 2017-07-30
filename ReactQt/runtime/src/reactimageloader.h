
/**
 * Copyright (C) 2016, Canonical Ltd.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * Author: Justin McPherson <justin.mcpherson@canonical.com>
 *
 */

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
  enum Event { Event_LoadStart, Event_Progress, Event_Error, Event_Load, Event_LoadEnd };
  typedef std::function<void(Event, const QVariantMap&)> LoadEventCallback;

  ReactImageLoader(QObject* parent = 0);
  ~ReactImageLoader();

  void setBridge(ReactBridge* bridge) override;

  QString moduleName() override;
  QList<ReactModuleMethod*> methodsToExport() override;
  QVariantMap constantsToExport() override;

  QUrl provideUriFromSourceUrl(const QUrl& source);
  void loadImage(const QUrl& source, const LoadEventCallback& ec = LoadEventCallback());

private:
  QScopedPointer<ReactImageLoaderPrivate> d_ptr;
};

#endif // REACTIMAGELOADER_H
