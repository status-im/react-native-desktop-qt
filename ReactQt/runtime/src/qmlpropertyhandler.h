/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#ifndef QMLPROPERTYHANDLER_H
#define QMLPROPERTYHANDLER_H

#include <QMap>
#include <QObject>
#include <QMetaProperty>
#include "reactpropertyhandler.h"

const QString QML_PROPERTY_PREFIX = "p_";

class QQuickItem;


class QmlPropertyHandler : public ReactPropertyHandler
{
  Q_OBJECT

public:
  QmlPropertyHandler(QObject* object);

private:
  virtual void buildPropertyMap() override;
  void getPropertiesFromMetaObject(const QMetaObject* metaObject);
};

#endif // QMLPROPERTYHANDLER_H
