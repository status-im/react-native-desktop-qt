
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#ifndef REACTSLIDERMANAGER_H
#define REACTSLIDERMANAGER_H

#include "reactviewmanager.h"

class ReactSliderManagerPrivate;
class ReactSliderManager : public ReactViewManager {
    Q_OBJECT
    Q_INTERFACES(ReactModuleInterface)
    Q_DECLARE_PRIVATE(ReactSliderManager)

public:
    ReactSliderManager(QObject* parent = 0);
    virtual ~ReactSliderManager();

    virtual ReactViewManager* viewManager() override;
    virtual QString moduleName() override;
    virtual QStringList customDirectEventTypes() override;

public slots:
    void sendSliderValueChangedToJs(QQuickItem* slider);
    void sendSlidingCompleteToJs(QQuickItem* slider);

private:
    virtual QString qmlComponentFile() const override;
    virtual void configureView(QQuickItem* view) const override;

    void notifyJsAboutSliderEvent(QQuickItem* slider, const QString& eventName) const;

private:
    QScopedPointer<ReactSliderManagerPrivate> d_ptr;
};

#endif // REACTSLIDERMANAGER_H
