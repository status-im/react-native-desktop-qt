
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
#include <QQuickItem>

PropertyHandler::PropertyHandler(QObject* object, SetPropertyCallback callback)
    : QObject(object), m_object(object), m_setPropertyCallback(callback) {
    Q_ASSERT(object);

    m_flexbox = Flexbox::findFlexbox(qobject_cast<QQuickItem*>(object));
}

PropertyHandler::~PropertyHandler() {}

QMap<QString, QMetaProperty> PropertyHandler::availableProperties() {
    buildPropertyMap();

    QMap<QString, QMetaProperty> allProps;
    allProps.unite(m_qmlProperties);
    allProps.unite(m_flexboxProperties);

    return allProps;
}

void PropertyHandler::applyProperties(const QVariantMap& properties) {
    buildPropertyMap();
    // qDebug() << __PRETTY_FUNCTION__ << m_object << properties;

    for (const QString& key : properties.keys()) {
        QVariant propertyValue = properties.value(key);

        auto it = m_qmlProperties.find(key);
        if (it != m_qmlProperties.end()) {
            QMetaProperty property = it.value();
            setValueToObjectProperty(m_object, property, propertyValue, m_defaultQmlValues[key]);
        }

        if (m_flexbox) {
            it = m_flexboxProperties.find(key);
            if (it != m_flexboxProperties.end()) {
                QMetaProperty property = it.value();
                setValueToObjectProperty(m_flexbox, property, propertyValue, m_defaultFlexboxValues[key]);
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
    buildPropertyMap();

    QVariant value;

    if (m_flexbox && m_flexboxProperties.contains(propertyName)) {
        value = m_flexboxProperties[propertyName].read(m_flexbox);
    } else if (m_qmlProperties.contains(propertyName)) {
        value = m_qmlProperties[propertyName].read(m_object);
    }

    return value;
}

void PropertyHandler::buildPropertyMap() {
    if (m_cached) {
        return;
    }

    // Get qml properties of current object (and its parents)
    getPropertiesFromObject(m_object, m_qmlProperties, m_defaultQmlValues);

    if (m_flexbox) {
        getPropertiesFromObject(m_flexbox, m_flexboxProperties, m_defaultFlexboxValues);
    }

    m_cached = true;
}

void PropertyHandler::getPropertiesFromObject(const QObject* object,
                                              QMap<QString, QMetaProperty>& propertiesMap,
                                              QMap<QString, QVariant>& defaultValuesMap) {

    const QMetaObject* metaObject = object->metaObject();

    // we get all prefixed properties from object and its parents
    const int propertyCount = metaObject->propertyCount();
    for (int i = 0; i < propertyCount; ++i) {
        QMetaProperty p = metaObject->property(i);
        QString qmlPropName = p.name();
        if (p.isScriptable() && qmlPropName.startsWith(QML_PROPERTY_PREFIX)) {
            QString nameWithoutPrefix = qmlPropName.right(qmlPropName.length() - QML_PROPERTY_PREFIX.length());
            propertiesMap.insert(nameWithoutPrefix, p);
            defaultValuesMap.insert(nameWithoutPrefix, p.read(object));
        }
    }
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
