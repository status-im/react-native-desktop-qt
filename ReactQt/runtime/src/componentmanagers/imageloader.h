
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

#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include <QUrl>

#include "moduleinterface.h"

class ImageLoaderPrivate;
class ImageLoader : public QObject, public ModuleInterface {
    Q_OBJECT
    // Q_PLUGIN_METADATA(IID ReactModuleInterface_IID)
    Q_INTERFACES(ModuleInterface)

    Q_INVOKABLE REACT_PROMISE void prefetchImage(const QString& url, double success, double error);

    Q_INVOKABLE REACT_PROMISE void getSize(const QString& url, double success, double error);

    Q_DECLARE_PRIVATE(ImageLoader)

public:
    enum Event { Event_LoadStart, Event_Progress, Event_LoadError, Event_LoadSuccess, Event_LoadEnd };

    typedef std::function<void(Event, const QVariantMap&)> LoadEventCallback;

    ImageLoader(QObject* parent = 0);
    ~ImageLoader();

    void setBridge(Bridge* bridge) override;

    QString moduleName() override;

    void loadImage(const QUrl& source, const LoadEventCallback& ec = LoadEventCallback());

private:
    QScopedPointer<ImageLoaderPrivate> d_ptr;
};

#endif // IMAGELOADER_H
