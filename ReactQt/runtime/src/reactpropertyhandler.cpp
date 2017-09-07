
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

#include <QDebug>

#include "reactpropertyhandler.h"
#include "reactvaluecoercion.h"


ReactPropertyHandler::ReactPropertyHandler(QObject* object)
  : QObject(object)
  , m_object(object)
{

}

ReactPropertyHandler::~ReactPropertyHandler()
{
}

QMap<QString, QMetaProperty> ReactPropertyHandler::availableProperties()
{
  buildPropertyMap();

  QMap<QString, QMetaProperty> allProperties;
  allProperties.unite(m_qmlProperties);
  allProperties.unite(m_HandlerProperties);
  return allProperties;
}

void ReactPropertyHandler::applyProperties(const QVariantMap& properties)
{
  buildPropertyMap();
  // qDebug() << __PRETTY_FUNCTION__ << m_object << properties;

  for (const QString& key : properties.keys()) {
    QVariant propertyValue = properties.value(key);
    QMap<QString, QMetaProperty>::iterator it = m_HandlerProperties.find(key);
    // Extras get first shot
    if (it != m_HandlerProperties.end()) {
      QMetaProperty property = it.value();
      setValueToObjectProperty(this, property, propertyValue);
    } else if (m_exposeQmlProperties) {
      it = m_qmlProperties.find(key);
      if (it != m_qmlProperties.end()) {
        QMetaProperty property = it.value();
        setValueToObjectProperty(m_object, property, propertyValue);
      }
    }
  }
}


QVariant ReactPropertyHandler::value(const QString& propertyName)
{
  buildPropertyMap();

  QVariant value;

  if(m_HandlerProperties.contains(propertyName))
  {
    value = m_HandlerProperties[propertyName].read(this);
  }
  else if(m_exposeQmlProperties && m_qmlProperties.contains(propertyName))
  {
    value = m_qmlProperties[propertyName].read(m_object);
  }
  return value;
}


void ReactPropertyHandler::buildPropertyMap()
{
  if (m_cached) {
    return;
  }

  // Get qml properties of current object (and its parents)
  if (m_exposeQmlProperties) {
    const QMetaObject* metaObject = m_object->metaObject();
    getPropertiesFromMetaObject(metaObject);
  }

  // Get all properties on the handlers (extras)
  const QMetaObject* metaObject = this->metaObject();
  const int propertyCount = metaObject->propertyCount();

  for (int i = 1; i < propertyCount; ++i) {
    QMetaProperty p = metaObject->property(i);
    if (p.isScriptable())
      m_HandlerProperties.insert(p.name(), p);
  }

  m_cached = true;
}


void ReactPropertyHandler::getPropertiesFromMetaObject(const QMetaObject* metaObject)
{
  //we get all prefixed properties from object and its parents
  const int propertyCount = metaObject->propertyCount();
  for (int i = 0; i < propertyCount; ++i) {
    QMetaProperty p = metaObject->property(i);
    QString qmlPropName = p.name();
    if (p.isScriptable() && qmlPropName.startsWith(QML_PROPERTY_PREFIX))
    {
      QString nameWithoutPrefix = qmlPropName.right(qmlPropName.length() - QML_PROPERTY_PREFIX.length());
      m_qmlProperties.insert(nameWithoutPrefix, p);
    }
  }
}


void ReactPropertyHandler::setValueToObjectProperty(QObject* object, QMetaProperty property, const QVariant& value)
{
  property.write(object, reactCoerceValue(value, property.userType()));
}
