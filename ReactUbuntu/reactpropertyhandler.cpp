#include <QDebug>

#include "reactpropertyhandler.h"
#include "reactvaluecoercion.h"


ReactPropertyHandler::ReactPropertyHandler(QObject* object)
  : QObject(object)
  , m_cached(false)
  , m_object(object)
{
}

ReactPropertyHandler::~ReactPropertyHandler()
{
}

QList<QMetaProperty> ReactPropertyHandler::availableProperties()
{
  return m_coreProperties.values() + m_extraProperties.values();
}

void ReactPropertyHandler::applyProperties(const QVariantMap& properties)
{
  buildPropertyMap();

  for (const QString& key : properties.keys()) {
    QMap<QString, QMetaProperty>::iterator it = m_extraProperties.find(key);
    // Extras get first shot
    if (it != m_extraProperties.end()) {
      it.value().write(this, reactCoerceValue(properties.value(key), it.value().userType()));
    } else {
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
  {
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

  // Snaffle up QObject::objectName as well, leave for now, might be good for testing
  for (int i = 0; i < propertyCount; ++i) {
    QMetaProperty p = metaObject->property(i);
    if (p.isScriptable())
      m_extraProperties.insert(p.name(), p);
  }
  }
  m_cached = true;
}
