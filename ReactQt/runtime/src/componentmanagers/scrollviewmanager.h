
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

#ifndef SCROLLVIEWMANAGER_H
#define SCROLLVIEWMANAGER_H

#include "viewmanager.h"

// #define QT_STATICPLUGIN

class ScrollViewManager : public ViewManager {
    Q_OBJECT
    // Q_PLUGIN_METADATA(IID ReactModuleInterface_IID)
    Q_INTERFACES(ModuleInterface)

    Q_INVOKABLE void scrollTo(int reactTag, double offsetX, double offsetY, bool animated);
    Q_INVOKABLE void scrollToEnd(int reactTag, bool animated);

public:
    ScrollViewManager(QObject* parent = 0);
    ~ScrollViewManager();

    ViewManager* viewManager() override;
    QString moduleName() override;

    QStringList customDirectEventTypes() override;

    void addChildItem(QQuickItem* scrollView, QQuickItem* child, int position) const override;

    static bool isArrayScrollingOptimizationEnabled(QQuickItem* item);
    static void updateListViewItem(QQuickItem* item, QQuickItem* child, int position);
    static void
    removeListViewItem(QQuickItem* item, const QList<int>& removeAtIndices, bool unregisterAndDelete = true);
    static QQuickItem* scrollViewContentItem(QQuickItem* item, int position);

private Q_SLOTS:
    void scrollBeginDrag();
    void scrollEndDrag();
    void scroll();

    void momentumScrollBegin();
    void momentumScrollEnd();

private:
    QVariantMap buildEventData(QQuickItem* item) const;
    virtual void configureView(QQuickItem* view) const override;
    virtual QString qmlComponentFile(const QVariantMap& properties) const override;
    bool arrayScrollingOptimizationEnabled(QQuickItem* item) const;

    static QMap<QQuickItem*, QQuickItem*> m_scrollViewByListViewItem;
    static QMap<QQuickItem*, QVariantList> m_modelByScrollView;
};

#endif // SCROLLVIEWMANAGER_H
