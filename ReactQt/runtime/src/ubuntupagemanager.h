
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

#ifndef UBUNTUPAGEMANAGER_H
#define UBUNTUPAGEMANAGER_H

#include <QString>
#include <QMap>

#include "reactviewmanager.h"


class QQuickItem;

class UbuntuPageManager : public ReactViewManager
{
  Q_OBJECT
  Q_INTERFACES(ReactModuleInterface)

public:
  UbuntuPageManager(QObject* parent = 0);
  ~UbuntuPageManager();

  void setBridge(ReactBridge* bridge) override;

  ReactViewManager* viewManager() override;
  ReactPropertyHandler* propertyHandler(QObject* object);

  QString moduleName() override;
  QList<ReactModuleMethod*> methodsToExport() override;
  QVariantMap constantsToExport() override;

  QQuickItem* view(const QVariantMap& properties) const override;

private Q_SLOTS:
  void widthChanged();
  void heightChanged();

private:
  void configureView(QQuickItem* view) const;

  mutable int m_id;
  QMap<QPair<QString, QQuickItem*>, QMetaMethod> m_methodCache;
};

#endif // UBUNTUPAGEMANAGER_H
