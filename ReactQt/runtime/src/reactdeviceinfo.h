/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#ifndef REACTDEVICEINFO_H
#define REACTDEVICEINFO_H

#include "reactmoduleinterface.h"

#include <QVariantMap>

class ReactDeviceInfoPrivate;
class ReactDeviceInfo
  : public QObject
  , public ReactModuleInterface
{
  Q_OBJECT
  Q_INTERFACES(ReactModuleInterface)

  Q_DECLARE_PRIVATE(ReactDeviceInfo)

public:
  ReactDeviceInfo(QObject* parent = 0);
  ~ReactDeviceInfo();

  void setBridge(ReactBridge* bridge) override;

  QString moduleName() override;
  QList<ReactModuleMethod*> methodsToExport() override;
  QVariantMap constantsToExport() override;

private:
  QScopedPointer<ReactDeviceInfoPrivate> d_ptr;
};

#endif // REACTDEVICEINFO_H
