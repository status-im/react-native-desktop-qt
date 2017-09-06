
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
  static QString textWithProperties(QQuickItem* item, const PropertyMap& properties);
  static QString convertPropsToHtml(const QString& text, const QVariantMap& properties);
  static PropertyMap props(QQuickItem* item);

private:
  virtual QString qmlComponentFile() const;
  virtual void configureView(QQuickItem* view) const;
};

#endif // REACTTEXTMANAGER_H
