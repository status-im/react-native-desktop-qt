
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#ifndef SLIDERMANAGER_H
#define SLIDERMANAGER_H

#include "viewmanager.h"

class SliderManagerPrivate;
class SliderManager : public ViewManager {
    Q_OBJECT
    Q_INTERFACES(ModuleInterface)
    Q_DECLARE_PRIVATE(SliderManager)

public:
    SliderManager(QObject* parent = 0);
    virtual ~SliderManager();

    virtual ViewManager* viewManager() override;
    virtual QString moduleName() override;
    virtual QStringList customDirectEventTypes() override;

public slots:
    void sendSliderValueChangedToJs(QQuickItem* slider);
    void sendSlidingCompleteToJs(QQuickItem* slider);

private:
    virtual QString qmlComponentFile(const QVariantMap& properties) const override;
    virtual void configureView(QQuickItem* view) const override;

    void notifyJsAboutSliderEvent(QQuickItem* slider, const QString& eventName) const;

private:
    QScopedPointer<SliderManagerPrivate> d_ptr;
};

#endif // SLIDERMANAGER_H
