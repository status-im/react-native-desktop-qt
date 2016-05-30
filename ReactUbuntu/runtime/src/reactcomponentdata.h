
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

#ifndef REACTCOMPONENTDATA_H
#define REACTCOMPONENTDATA_H

#include <QString>
#include <QVariant>

class QQuickItem;
class ReactModuleData;
class ReactViewManager;
class ReactModuleInterface;
class ReactModuleMethod;


class ReactComponentData
{
public:
  ReactComponentData(ReactModuleData* moduleData);
  ~ReactComponentData();

  QString name() const;
  ReactViewManager* manager() const;

  QVariantMap viewConfig() const;

  QQuickItem* createView(int tag, const QVariantMap& properties);

  ReactModuleMethod* method(int id) const;

private:
  ReactModuleData* m_moduleData;
  ReactModuleInterface* m_moduleInterface;
};


#endif // REACTCOMPONENTDATA_H
