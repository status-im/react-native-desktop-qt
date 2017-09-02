
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

#ifndef REACTRAWTEXTMANAGER_H
#define REACTRAWTEXTMANAGER_H

#include <QVariant>

#include "reactviewmanager.h"

class QQuickItem;
class ReactPropertyHandler;
// #define QT_STATICPLUGIN

class ReactRawTextManager : public ReactViewManager
{
  Q_OBJECT
  // Q_PLUGIN_METADATA(IID ReactModuleInterface_IID)
  Q_INTERFACES(ReactModuleInterface)

public:
  ReactRawTextManager(QObject *parent = 0);
  ~ReactRawTextManager();

  // TODO: this doesnt seem right
  ReactViewManager* viewManager() override;
  ReactPropertyHandler* propertyHandler(QObject* object) override;

  QString moduleName() override;
  QList<ReactModuleMethod*> methodsToExport() override;
  QVariantMap constantsToExport() override;

  bool shouldLayout() const override;

private:

  virtual void configureView(QQuickItem* view) const;
  virtual QString qmlComponentFile() const;

};

#endif // REACTRAWTEXTMANAGER_H
