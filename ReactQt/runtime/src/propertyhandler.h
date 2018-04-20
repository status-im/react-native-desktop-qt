
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

#ifndef PROPERTYHANDLER_H
#define PROPERTYHANDLER_H

#include <functional>

#include <QMap>
#include <QMetaProperty>
#include <QObject>

class QQuickItem;
class Flexbox;
const QString QML_PROPERTY_PREFIX = "p_";

using SetPropertyCallback = std::function<void(QObject* object, QMetaProperty property, const QVariant& value)>;

class PropertyHandler : public QObject {
    Q_OBJECT

public:
    PropertyHandler(QObject* object, SetPropertyCallback callback = SetPropertyCallback());
    virtual ~PropertyHandler();

    virtual QMap<QString, QMetaProperty> availableProperties();
    virtual void applyProperties(const QVariantMap& properties);
    QVariant value(const QString& propertyName);

private:
    void buildPropertyMap();
    void setValueToObjectProperty(QObject* object,
                                  QMetaProperty property,
                                  const QVariant& value,
                                  const QVariant& defaultValue);
    void getPropertiesFromObject(const QObject* object,
                                 QMap<QString, QMetaProperty>& propertiesMap,
                                 QMap<QString, QVariant>& defaultValuesMap);

private:
    bool m_cached = false;
    QObject* m_object = nullptr;
    Flexbox* m_flexbox = nullptr;
    QMap<QString, QMetaProperty> m_qmlProperties;
    QMap<QString, QMetaProperty> m_flexboxProperties;
    QMap<QString, QVariant> m_defaultQmlValues;
    QMap<QString, QVariant> m_defaultFlexboxValues;
    SetPropertyCallback m_setPropertyCallback;
};

#endif // PROPERTYHANDLER_H
