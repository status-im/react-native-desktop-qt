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
  Q_INVOKABLE void measure(int reactTag, const ReactModuleInterface::ResponseBlock& callback);
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
  // Q_INVOKABLE void replaceExistingNonRootView();
  // Q_INVOKABLE void measureLayout();
  // Q_INVOKABLE void scrollTo();
  // Q_INVOKABLE void measureLayoutRelativeToParent();
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
                                 const ReactModuleInterface::ResponseBlock& callback);

public:
  ReactUIManager();
  ~ReactUIManager();

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
