
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

#ifndef REACTUIMANAGER_H
#define REACTUIMANAGER_H

#include <QString>
#include <QVariant>
#include <QObject>

#include "reactmoduleinterface.h"


class ReactBridge;
class ReactComponentData;


class ReactUIManager
  : public QObject
  , public ReactModuleInterface
{
  Q_OBJECT
  Q_INTERFACES(ReactModuleInterface)

  // Q_INVOKABLE void measureViewsInRect();
  // Q_INVOKABLE void scrollWithoutAnimationTo();
  Q_INVOKABLE void removeSubviewsFromContainerWithID(int containerReactTag);
  // Q_INVOKABLE void setMainScrollViewTag();
  // Q_INVOKABLE void focus();
  Q_INVOKABLE void measure(int reactTag, const ReactModuleInterface::ListArgumentBlock& callback);
  // Q_INVOKABLE void removeRootView();
  Q_INVOKABLE void updateView(int reactTag,
                              const QString& viewName,
                              const QVariantMap& properties);
  Q_INVOKABLE void manageChildren(int containerReactTag,
                                  const QList<int>& moveFromIndicies,
                                  const QList<int>& moveToIndices,
                                  const QList<int>& addChildReactTags,
                                  const QList<int>& addAtIndices,
                                  const QList<int>& removeAtIndices);
  Q_INVOKABLE void setChildren(int containerReactTag,
                               const QList<int>& childrenTags);

  Q_INVOKABLE void replaceExistingNonRootView(int reactTag, int newReactTag);
  Q_INVOKABLE void measureLayout(int reactTag,
                                 int ancestorReactTag,
                                 const ReactModuleInterface::ListArgumentBlock& errorCallback,
                                 const ReactModuleInterface::ListArgumentBlock& callback);
  // Q_INVOKABLE void scrollTo();
  Q_INVOKABLE void measureLayoutRelativeToParent(int reactTag,
                                                 const ReactModuleInterface::ListArgumentBlock& errorCallback,
                                                 const ReactModuleInterface::ListArgumentBlock& callback);
  // Q_INVOKABLE void configureNextLayoutAnimation();
  Q_INVOKABLE void blur(int reactTag);
  Q_INVOKABLE void setJSResponder(int reactTag, bool blockNativeResponder);
  Q_INVOKABLE void clearJSResponder();
  // Q_INVOKABLE void zoomToRect();
  Q_INVOKABLE void createView(int reactTag,
                              const QString& viewName,
                              int rootTag,
                              const QVariantMap& props);
  Q_INVOKABLE void findSubviewIn(int reactTag,
                                 const QPointF& point,
                                 const ReactModuleInterface::ListArgumentBlock& callback);
  Q_INVOKABLE void dispatchViewManagerCommand(int reactTag,
                                              int commandID,
                                              const QVariantList& commandArgs);

  Q_INVOKABLE REACT_PROMISE
    void takeSnapshot(const QString& target,
                      const QVariantMap& options,
                      const ReactModuleInterface::ListArgumentBlock& resolve,
                      const ReactModuleInterface::ListArgumentBlock& reject);

public:
  ReactUIManager();
  ~ReactUIManager();

  void reset();

  void setBridge(ReactBridge *bridge) override;

  QString moduleName() override;
  QList<ReactModuleMethod*> methodsToExport() override;
  QVariantMap constantsToExport() override;

  int allocateRootTag();

  void registerRootView(QQuickItem* root);

  QQuickItem* viewForTag(int reactTag);

public Q_SLOTS:
  void rootViewWidthChanged();
  void rootViewHeightChanged();
  void rootViewScaleChanged();

private:
  static int m_nextRootTag;

  ReactBridge* m_bridge;
  QMap<QString, ReactComponentData*> m_componentData;
  QMap<int, QQuickItem*> m_views;
};

#endif // REACTUIMANAGER_H
