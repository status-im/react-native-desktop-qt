
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
  // qDebug() << __PRETTY_FUNCTION__ << m_object << properties;
  buildPropertyMap();

  /*
  qDebug() << "Available properties";
  for (auto p : m_coreProperties) {
    qDebug() << p.name();
  }

  for (auto p : m_extraProperties) {
    qDebug() << p.name();
  }
  */

  for (const QString& key : properties.keys()) {
    QMap<QString, QMetaProperty>::iterator it = m_extraProperties.find(key);
    // Extras get first shot
    if (it != m_extraProperties.end()) {
      it.value().write(this, reactCoerceValue(properties.value(key), it.value().userType()));
    } else if (m_exposeQmlProperties) {
      it = m_coreProperties.find(key);
      if (it != m_coreProperties.end()) {
        it.value().write(m_object, reactCoerceValue(properties.value(key), it.value().userType()));
      }
    }
  }
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
