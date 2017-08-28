
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


ReactPropertyHandler::ReactPropertyHandler(QObject* object, bool exposeQmlProperties)
  : QObject(object)
  , m_exposeQmlProperties(exposeQmlProperties)
  , m_object(object)
{

}

ReactPropertyHandler::~ReactPropertyHandler()
{
}

QList<QMetaProperty> ReactPropertyHandler::availableProperties()
{
  buildPropertyMap();

  return m_coreProperties.values() + m_extraProperties.values();
}

void ReactPropertyHandler::applyProperties(const QVariantMap& properties)
{
  buildPropertyMap();
  // qDebug() << __PRETTY_FUNCTION__ << m_object << properties;

  for (const QString& key : properties.keys()) {
    QVariant propertyValue = properties.value(key);
    QMap<QString, QMetaProperty>::iterator it = m_extraProperties.find(key);
    // Extras get first shot
    if (it != m_extraProperties.end()) {
      QMetaProperty property = it.value();
      setValueToObjectProperty(this, property, propertyValue);
    } else if (m_exposeQmlProperties) {
      it = m_coreProperties.find(key);
      if (it != m_coreProperties.end()) {
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

  if(m_extraProperties.contains(propertyName))
  {
    value = m_extraProperties[propertyName].read(this);
  }
  else if(m_exposeQmlProperties && m_coreProperties.contains(propertyName))
  {
    value = m_coreProperties[propertyName].read(m_object);
  }
  return value;
}


void ReactPropertyHandler::buildPropertyMap()
{
  if (m_cached) {
    return;
  }

  // Class properties on the actual object (core object)
  if (m_exposeQmlProperties) {
    const QMetaObject* metaObject = m_object->metaObject();
    const int propertyCount = metaObject->propertyCount();

    for (int i = metaObject->propertyOffset(); i < propertyCount; ++i) {
      QMetaProperty p = metaObject->property(i);
      if (p.isScriptable())
        m_coreProperties.insert(p.name(), p);
    }
  }

  // All properties on the handlers (extras)
  {
  const QMetaObject* metaObject = this->metaObject();
  const int propertyCount = metaObject->propertyCount();

  for (int i = 1; i < propertyCount; ++i) {
    QMetaProperty p = metaObject->property(i);
    if (p.isScriptable())
      m_extraProperties.insert(p.name(), p);
  }
  }
  m_cached = true;
}

void ReactPropertyHandler::setValueToObjectProperty(QObject* object, QMetaProperty property, const QVariant& value)
{
  property.write(object, reactCoerceValue(value, property.userType()));
}
