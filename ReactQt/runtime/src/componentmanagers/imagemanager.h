
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

#ifndef IMAGEMANAGER_H
#define IMAGEMANAGER_H

#include "viewmanager.h"

class PropertyHandler;
class ImageManagerPrivate;
class ImageManager : public ViewManager {
    Q_OBJECT
    Q_INTERFACES(ModuleInterface)
    Q_DECLARE_PRIVATE(ImageManager)

public:
    ImageManager(QObject* parent = 0);
    virtual ~ImageManager();

    virtual ViewManager* viewManager() override;
    virtual QString moduleName() override;

    virtual QStringList customDirectEventTypes() override;

public slots:
    void manageSource(const QVariantMap& imageSource, QQuickItem* image);

private:
    virtual void configureView(QQuickItem* view) const override;
    virtual QString qmlComponentFile(const QVariantMap& properties) const override;

private:
    QScopedPointer<ImageManagerPrivate> d_ptr;
};

#endif // IMAGEMANAGER_H
