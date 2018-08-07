
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

#ifndef RAWTEXTMANAGER_H
#define RAWTEXTMANAGER_H

#include <QVariant>

#include "viewmanager.h"

class QQuickItem;
class PropertyHandler;
// #define QT_STATICPLUGIN

class RawTextManager : public ViewManager {
    Q_OBJECT
    // Q_PLUGIN_METADATA(IID ReactModuleInterface_IID)
    Q_INTERFACES(ModuleInterface)

public:
    RawTextManager(QObject* parent = 0);
    ~RawTextManager();

    // TODO: this doesnt seem right
    ViewManager* viewManager() override;

    QString moduleName() override;

    bool shouldLayout() const override;

protected:
    virtual void configureView(QQuickItem* view) const override;
    virtual QString qmlComponentFile(const QVariantMap& properties) const override;
};

#endif // RAWTEXTMANAGER_H
