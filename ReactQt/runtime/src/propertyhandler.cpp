
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

#include "layout/flexbox.h"
#include "propertyhandler.h"
#include "valuecoercion.h"
#include <QMutexLocker>
#include <QQuickItem>

QMap<QString, Props*> PropertyHandler::m_propsForClass = QMap<QString, Props*>();
QMutex PropertyHandler::m_mutex;

PropertyHandler::PropertyHandler(QObject* object, SetPropertyCallback callback)
    : QObject(object), m_object(object), m_setPropertyCallback(callback) {
    Q_ASSERT(object);

    m_flexbox = Flexbox::findFlexbox(qobject_cast<QQuickItem*>(object));
    m_className = object->metaObject()->className();
}

PropertyHandler::~PropertyHandler() {}

QMap<QString, QMetaProperty> PropertyHandler::availableProperties() {
    QMap<QString, QMetaProperty> allProps;
    allProps.unite(*qmlProperties());
    allProps.unite(*flexboxProperties());

    return allProps;
}

void PropertyHandler::applyProperties(const QVariantMap& properties) {

    for (const QString& key : properties.keys()) {
        QVariant propertyValue = properties.value(key);

        //        qDebug() << "SET PROP CLASS: " << m_className << " ADDRESS:" <<
        //        m_propsForClass[m_className]->m_qmlProperties;

        auto it = qmlProperties()->find(key);
        if (it != qmlProperties()->end()) {
            QMetaProperty property = it.value();
            setValueToObjectProperty(m_object, property, propertyValue, defaultQmlValues()->value(key));
        }

        if (m_flexbox) {
            it = flexboxProperties()->find(key);
            if (it != flexboxProperties()->end()) {
                QMetaProperty property = it.value();
                setValueToObjectProperty(m_flexbox, property, propertyValue, defaultFlexboxValues()->value(key));
            }
        }
    }

    // when all props received from js are set we notify components
    const QMetaObject* meta = m_object->metaObject();
    int jsPropsSetMethodIndex = meta->indexOfMethod("onJsPropertiesSet()");
    if (jsPropsSetMethodIndex != -1) {
        meta->method(jsPropsSetMethodIndex).invoke(m_object);
    }
}

QVariant PropertyHandler::value(const QString& propertyName) {
    QVariant value;

    if (m_flexbox && flexboxProperties()->contains(propertyName)) {
        value = flexboxProperties()->value(propertyName).read(m_flexbox);
    } else if (qmlProperties()->contains(propertyName)) {
        value = qmlProperties()->value(propertyName).read(m_object);
    }

    return value;
}

void PropertyHandler::buildPropertyMap() {
    if (m_propsForClass.contains(m_className)) {
        return;
    } else {
        Props* properties = new Props();
        m_propsForClass[m_className] = properties;

        // Get qml properties of current object (and its parents)
        getPropertiesFromObject(m_object, properties->m_qmlProperties, properties->m_defaultQmlValues);

        if (m_flexbox) {
            getPropertiesFromObject(m_flexbox, properties->m_flexboxProperties, properties->m_defaultFlexboxValues);
        }
    }
}

void PropertyHandler::getPropertiesFromObject(const QObject* object,
                                              QMap<QString, QMetaProperty>* propertiesMap,
                                              QMap<QString, QVariant>* defaultValuesMap) {

    const QMetaObject* metaObject = object->metaObject();

    // we get all prefixed properties from object and its parents
    const int propertyCount = metaObject->propertyCount();
    for (int i = 0; i < propertyCount; ++i) {
        QMetaProperty p = metaObject->property(i);
        QString qmlPropName = p.name();
        if (p.isScriptable() && qmlPropName.startsWith(QML_PROPERTY_PREFIX)) {
            QString nameWithoutPrefix = qmlPropName.right(qmlPropName.length() - QML_PROPERTY_PREFIX.length());
            propertiesMap->insert(nameWithoutPrefix, p);
            //            qDebug() << "PROP CLASS: " << m_className << " ADDRESS:" <<
            //            m_propsForClass[m_className]->m_qmlProperties;
            defaultValuesMap->insert(nameWithoutPrefix, p.read(object));
        }
    }
}

const QMap<QString, QMetaProperty>* PropertyHandler::qmlProperties() {
    if (!m_propsForClass.contains(m_className)) {
        buildPropertyMap();
    }
    return m_propsForClass[m_className]->m_qmlProperties;
}

const QMap<QString, QMetaProperty>* PropertyHandler::flexboxProperties() {
    if (!m_propsForClass.contains(m_className)) {
        buildPropertyMap();
    }
    return m_propsForClass[m_className]->m_flexboxProperties;
}

const QMap<QString, QVariant>* PropertyHandler::defaultQmlValues() {
    if (!m_propsForClass.contains(m_className)) {
        buildPropertyMap();
    }
    return m_propsForClass[m_className]->m_defaultQmlValues;
}

const QMap<QString, QVariant>* PropertyHandler::defaultFlexboxValues() {
    if (!m_propsForClass.contains(m_className)) {
        buildPropertyMap();
    }
    return m_propsForClass[m_className]->m_defaultFlexboxValues;
}

void PropertyHandler::setValueToObjectProperty(QObject* object,
                                               QMetaProperty property,
                                               const QVariant& value,
                                               const QVariant& defaultValue) {
    QVariant newValue;
    if (value.isNull()) {
        newValue = defaultValue;
    } else {
        newValue = reactCoerceValue(value, property.userType());
    }
    property.write(object, newValue);

    if (m_setPropertyCallback) {
        m_setPropertyCallback(object, property, value);
    }
}
