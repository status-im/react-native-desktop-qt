
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

#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <QLoggingCategory>
#include <QObject>
#include <QString>
#include <QVariant>

#include "moduleinterface.h"

Q_DECLARE_LOGGING_CATEGORY(UIMANAGER)

class Bridge;
class ComponentData;
class QQuickItem;

class UIManager : public QObject, public ModuleInterface {
    Q_OBJECT
    Q_INTERFACES(ModuleInterface)

    // Q_INVOKABLE void measureViewsInRect();
    // Q_INVOKABLE void scrollWithoutAnimationTo();
    Q_INVOKABLE void removeSubviewsFromContainerWithID(int containerReactTag);
    // Q_INVOKABLE void setMainScrollViewTag();
    // Q_INVOKABLE void focus();
    Q_INVOKABLE void measure(int reactTag, const ModuleInterface::ListArgumentBlock& callback);
    Q_INVOKABLE void measureInWindow(int reactTag, const ModuleInterface::ListArgumentBlock& callback);
    // Q_INVOKABLE void removeRootView();
    Q_INVOKABLE void updateView(int reactTag, const QString& viewName, const QVariantMap& properties);
    Q_INVOKABLE void manageChildren(int containerReactTag,
                                    const QList<int>& moveFromIndicies,
                                    const QList<int>& moveToIndices,
                                    const QList<int>& addChildReactTags,
                                    const QList<int>& addAtIndices,
                                    const QList<int>& removeAtIndices);
    Q_INVOKABLE void setChildren(int containerReactTag, const QList<int>& childrenTags);

    Q_INVOKABLE void replaceExistingNonRootView(int reactTag, int newReactTag);
    Q_INVOKABLE void measureLayout(int reactTag,
                                   int ancestorReactTag,
                                   const ModuleInterface::ListArgumentBlock& errorCallback,
                                   const ModuleInterface::ListArgumentBlock& callback);
    // Q_INVOKABLE void scrollTo();
    Q_INVOKABLE void measureLayoutRelativeToParent(int reactTag,
                                                   const ModuleInterface::ListArgumentBlock& errorCallback,
                                                   const ModuleInterface::ListArgumentBlock& callback);
    // Q_INVOKABLE void configureNextLayoutAnimation();
    Q_INVOKABLE void blur(int reactTag);
    Q_INVOKABLE void setJSResponder(int reactTag, bool blockNativeResponder);
    Q_INVOKABLE void clearJSResponder();
    // Q_INVOKABLE void zoomToRect();
    Q_INVOKABLE void createView(int reactTag, const QString& viewName, int rootTag, const QVariantMap& props);
    Q_INVOKABLE void
    findSubviewIn(int reactTag, const QPointF& point, const ModuleInterface::ListArgumentBlock& callback);
    Q_INVOKABLE void dispatchViewManagerCommand(int reactTag, int commandID, const QVariantList& commandArgs);

    Q_INVOKABLE REACT_PROMISE void takeSnapshot(const QString& target,
                                                const QVariantMap& options,
                                                const ModuleInterface::ListArgumentBlock& resolve,
                                                const ModuleInterface::ListArgumentBlock& reject);

public:
    UIManager();
    ~UIManager();

    void reset();

    void setBridge(Bridge* bridge) override;

    QString moduleName() override;
    QVariantMap constantsToExport() override;

    int allocateRootTag();

    void registerRootView(QQuickItem* root);

    QQuickItem* viewForTag(int reactTag);
    void stopTrackingTagsForHierarchy(QQuickItem* topItem);

private:
    QList<QQuickItem*> removeChildrenFromVisualParent(QQuickItem* parent, const QList<int>& removeAtIndices);
    void destroyComponents(const QList<QQuickItem*> items);

private:
    static int m_nextRootTag;

    Bridge* m_bridge;
    QMap<QString, ComponentData*> m_componentData;
    QMap<int, QQuickItem*> m_views;
    int m_rootTag = -1;
};

#endif // UIMANAGER_H
