
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#ifndef SWITCHMANAGER_H
#define SWITCHMANAGER_H

#include "viewmanager.h"

class PropertyHandler;
class SwitchManagerPrivate;
class SwitchManager : public ViewManager {
    Q_OBJECT
    Q_INTERFACES(ModuleInterface)
    Q_DECLARE_PRIVATE(SwitchManager)

public:
    SwitchManager(QObject* parent = 0);
    virtual ~SwitchManager();

    virtual ViewManager* viewManager() override;
    virtual QString moduleName() override;

    virtual QStringList customDirectEventTypes() override;
    virtual bool shouldLayout() const override;

public slots:
    void sendValueChangeToJs(QQuickItem* picker, bool value);

private:
    virtual QString qmlComponentFile(const QVariantMap& properties) const override;
    virtual void configureView(QQuickItem* view) const override;
    void updateMeasureFunction(QQuickItem* view) const;

private:
    QScopedPointer<SwitchManagerPrivate> d_ptr;
};

#endif // SWITCHMANAGER_H
