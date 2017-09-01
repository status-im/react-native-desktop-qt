/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include <QDebug>

#include "qmlpropertyhandler.h"
#include "reactvaluecoercion.h"

QmlPropertyHandler::QmlPropertyHandler(QObject* object)
  : ReactPropertyHandler(object, true)
{

}

void QmlPropertyHandler::buildPropertyMap()
{
  if (m_cached) {
    return;
  }

  // Get qml properties of current object (and its parents)
  if (m_exposeQmlProperties) {
    const QMetaObject* metaObject = m_object->metaObject();
    getPropertiesFromMetaObject(metaObject);
  }

  // All properties on the handlers (extras)
  {
  const QMetaObject* metaObject = this->metaObject();
  const int propertyCount = metaObject->propertyCount();

  for (int i = 1; i < propertyCount; ++i) {
    QMetaProperty p = metaObject->property(i);
    if (p.isScriptable())
      m_HandlerProperties.insert(p.name(), p);
  }
  }
  m_cached = true;
}

void QmlPropertyHandler::getPropertiesFromMetaObject(const QMetaObject* metaObject)
{
  //qDebug()<<"----------getProperties-------";
  //we get all prefixed properties from object and its parents
  const int propertyCount = metaObject->propertyCount();
  for (int i = 0 /*metaObject->propertyOffset()*/; i < propertyCount; ++i) {
    QMetaProperty p = metaObject->property(i);
    QString qmlPropName = p.name();
    if (p.isScriptable() && qmlPropName.startsWith(QML_PROPERTY_PREFIX))
    {
      //qDebug()<<"exposed: "<<qmlPropName;
      QString nameWithoutPrefix = qmlPropName.right(qmlPropName.length() - QML_PROPERTY_PREFIX.length());
      m_qmlProperties.insert(nameWithoutPrefix, p);
    }
  }
}
