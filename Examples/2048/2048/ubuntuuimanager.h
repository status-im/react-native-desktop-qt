#ifndef UBUNTUUIMANAGER_H
#define UBUNTUUIMANAGER_H

#include <QString>
#include <QVariant>
#include <QObject>

// #define QT_STATICPLUGIN

#include "reactmoduleinterface.h"

class ReactBridge;
class ReactComponentData;

class UbuntuUIManager
  : public QObject
  , public ReactModuleInterface
{
  Q_OBJECT
  // Q_PLUGIN_METADATA(IID ReactModuleInterface_IID)
  Q_INTERFACES(ReactModuleInterface)

  // Q_INVOKABLE void measureViewsInRect();
  // Q_INVOKABLE void scrollWithoutAnimationTo();
  // Q_INVOKABLE void removeSubviewsFromContainerWithID();
  // Q_INVOKABLE void setMainScrollViewTag();
  // Q_INVOKABLE void focus();
  // Q_INVOKABLE void measure();
  // Q_INVOKABLE void removeRootView();
  Q_INVOKABLE void updateView(int reactTag,
                              const QString& viewName,
                              const QVariantMap& properties);
  Q_INVOKABLE void manageChildren(int containerReactTag,
                                  const QVariantList& moveFromIndicies,
                                  const QVariantList& moveToIndices,
                                  const QVariantList& addChildReactTags,
                                  const QVariantList& addAtIndices,
                                  const QVariantList& removeAtIndices);
  // Q_INVOKABLE void replaceExistingNonRootView();
  // Q_INVOKABLE void measureLayout();
  // Q_INVOKABLE void scrollTo();
  // Q_INVOKABLE void measureLayoutRelativeToParent();
  // Q_INVOKABLE void configureNextLayoutAnimation();
  // Q_INVOKABLE void blur();
  // Q_INVOKABLE void clearJSResponder();
  // Q_INVOKABLE void setJSResponder();
  // Q_INVOKABLE void zoomToRect();
  Q_INVOKABLE void createView(int reactTag,
                              const QString& viewName,
                              int rootTag,
                              const QVariantMap& props);
  //  Q_INVOKABLE void findSubviewIn();

public:
  UbuntuUIManager();
  ~UbuntuUIManager();

  void setBridge(ReactBridge *bridge);

  QString moduleName();
  QStringList methodsToExport();
  QVariantMap constantsToExport();

private:
  ReactBridge* m_bridge;
  QMap<QString, ReactComponentData*> m_componentData;
};

#endif // UBUNTUUIMANAGER_H
