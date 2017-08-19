/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#ifndef REACTTESTMODULE_H
#define REACTTESTMODULE_H

#include "reactmoduleinterface.h"

#include <QVariantMap>

class ReactTestModulePrivate;
class ReactTestModule
  : public QObject
  , public ReactModuleInterface
{
  Q_OBJECT
  Q_INTERFACES(ReactModuleInterface)

  Q_DECLARE_PRIVATE(ReactTestModule)

public:
  ReactTestModule(QObject* parent = 0);
  ~ReactTestModule();

  void setBridge(ReactBridge* bridge) override;

  QString moduleName() override;
  QList<ReactModuleMethod*> methodsToExport() override;
  QVariantMap constantsToExport() override;

  Q_INVOKABLE void markTestCompleted();

signals:
  void testCompleted();

private:
  QScopedPointer<ReactTestModulePrivate> d_ptr;
};

#endif // REACTTESTMODULE_H
