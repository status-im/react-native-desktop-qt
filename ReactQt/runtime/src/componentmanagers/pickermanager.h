
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#ifndef PICKERMANAGER_H
#define PICKERMANAGER_H

#include "viewmanager.h"

class PropertyHandler;
class PickerManagerPrivate;
class PickerManager : public ViewManager {
    Q_OBJECT
    Q_INTERFACES(ModuleInterface)
    Q_DECLARE_PRIVATE(PickerManager)

public:
    PickerManager(QObject* parent = 0);
    virtual ~PickerManager();

    virtual ViewManager* viewManager() override;
    virtual QString moduleName() override;

    virtual QStringList customDirectEventTypes() override;
    virtual bool shouldLayout() const override;

public slots:
    void sendValueChangeToJs(QQuickItem* picker, int index);

private:
    virtual QString qmlComponentFile(const QVariantMap& properties) const override;
    virtual void configureView(QQuickItem* view) const override;

private:
    QScopedPointer<PickerManagerPrivate> d_ptr;
};

#endif // PICKERMANAGER_H
