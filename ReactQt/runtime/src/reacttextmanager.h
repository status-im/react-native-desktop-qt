
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

#ifndef REACTTEXTMANAGER_H
#define REACTTEXTMANAGER_H

#include "reactrawtextmanager.h"

// #define QT_STATICPLUGIN

class ReactTextManager : public ReactRawTextManager
{
  Q_OBJECT
  // Q_PLUGIN_METADATA(IID ReactModuleInterface_IID)
  Q_INTERFACES(ReactModuleInterface)

public:
  ReactTextManager(QObject* parent = 0);
  ~ReactTextManager();

  ReactViewManager* viewManager() override;
  ReactPropertyHandler* propertyHandler(QObject* object) override;

  QString moduleName() override;
  QList<ReactModuleMethod*> methodsToExport() override;
  QVariantMap constantsToExport() override;

  bool shouldLayout() const override;


  static void hookLayout(QQuickItem* textItem);
  typedef std::map<QString, QVariant> PropertyMap;
  static void resizeToWidth(QQuickItem* textItem, double width);

public slots:
  QVariant nestedPropertyValue(QQuickItem* item, const QString& propertyName);

private:
  QQuickItem* parentTextItem(QQuickItem* textItem);
  bool propertyExplicitlySet(QQuickItem* item, const QString& propertyName);

  virtual QString qmlComponentFile() const;
  virtual void configureView(QQuickItem* view) const;

private:
  using StringSet = QSet<QString>;
  using PropertiesMap = QMap<QObject*, StringSet>;

  PropertiesMap m_explicitlySetProps;

};

#endif // REACTTEXTMANAGER_H
