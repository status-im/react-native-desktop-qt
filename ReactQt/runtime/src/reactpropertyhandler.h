
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

#ifndef REACTPROPERTYHANDLER_H
#define REACTPROPERTYHANDLER_H

#include <functional>

#include <QMap>
#include <QMetaProperty>
#include <QObject>

class QQuickItem;
class Flexbox;
const QString QML_PROPERTY_PREFIX = "p_";

using SetPropertyCallback = std::function<void(QObject* object, QMetaProperty property, const QVariant& value)>;

class ReactPropertyHandler : public QObject {
    Q_OBJECT

public:
    ReactPropertyHandler(QObject* object, SetPropertyCallback callback = SetPropertyCallback());
    virtual ~ReactPropertyHandler();

    virtual QMap<QString, QMetaProperty> availableProperties();
    virtual void applyProperties(const QVariantMap& properties);
    QVariant value(const QString& propertyName);

private:
    void buildPropertyMap();
    void setValueToObjectProperty(QObject* object, QMetaProperty property, const QVariant& value);
    void getPropertiesFromMetaObject(const QMetaObject* metaObject, QMap<QString, QMetaProperty>& propertiesMap);

private:
    bool m_cached = false;
    QObject* m_object = nullptr;
    Flexbox* m_flexbox = nullptr;
    QMap<QString, QMetaProperty> m_qmlProperties;
    QMap<QString, QMetaProperty> m_flexboxProperties;
    SetPropertyCallback m_setPropertyCallback;
};

#endif // REACTPROPERTYHANDLER_H
