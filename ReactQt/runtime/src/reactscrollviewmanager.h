
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

#ifndef REACTSCROLLVIEWMANAGER_H
#define REACTSCROLLVIEWMANAGER_H

#include "reactviewmanager.h"

// #define QT_STATICPLUGIN

class ReactScrollViewManager : public ReactViewManager {
    Q_OBJECT
    // Q_PLUGIN_METADATA(IID ReactModuleInterface_IID)
    Q_INTERFACES(ReactModuleInterface)

    Q_INVOKABLE void scrollTo(int reactTag, double offsetX, double offsetY, bool animated);

public:
    ReactScrollViewManager(QObject* parent = 0);
    ~ReactScrollViewManager();

    ReactViewManager* viewManager() override;
    ReactPropertyHandler* propertyHandler(QObject* object) override;

    QString moduleName() override;
    QList<ReactModuleMethod*> methodsToExport() override;
    QVariantMap constantsToExport() override;

    QStringList customDirectEventTypes() override;

    void addChildItem(QQuickItem* scrollView, QQuickItem* child, int position) const override;

private Q_SLOTS:
    void scrollBeginDrag();
    void scrollEndDrag();
    void scroll();

    void momentumScrollBegin();
    void momentumScrollEnd();

private:
    QVariantMap buildEventData(QQuickItem* item) const;
    virtual void configureView(QQuickItem* view) const;
    virtual QString qmlComponentFile() const;
};

#endif // REACTSCROLLVIEWMANAGER_H
