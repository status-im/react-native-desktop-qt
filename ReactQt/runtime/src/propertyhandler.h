
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
#include <QMutex>
#include <QObject>

class QQuickItem;
class Flexbox;
const QString QML_PROPERTY_PREFIX = "p_";

struct Props {

    Props() {
        m_qmlProperties = new QMap<QString, QString>();
        m_flexboxProperties = new QMap<QString, QString>();
        m_defaultQmlValues = new QMap<QString, QVariant>();
        m_defaultFlexboxValues = new QMap<QString, QVariant>();
    };

    ~Props() {
        delete m_qmlProperties;
        delete m_flexboxProperties;
        delete m_defaultQmlValues;
        delete m_defaultFlexboxValues;
    }

    QMap<QString, QString>* m_qmlProperties = nullptr;
    QMap<QString, QString>* m_flexboxProperties = nullptr;
    QMap<QString, QVariant>* m_defaultQmlValues = nullptr;
    QMap<QString, QVariant>* m_defaultFlexboxValues = nullptr;
};

using SetPropertyCallback = std::function<void(QObject* object, const QString& propertyName, const QVariant& value)>;

class PropertyHandler : public QObject {
    Q_OBJECT

public:
    PropertyHandler(QObject* object, SetPropertyCallback callback = SetPropertyCallback());
    virtual ~PropertyHandler();

    virtual QMap<QString, QString> availableProperties();
    virtual void applyProperties(const QVariantMap& properties);
    QMetaProperty metaProperty(const QString& propertyName);

private:
    void buildPropertyMap();
    void setValueToObjectProperty(QObject* object,
                                  const QString& property,
                                  const QString& qmlPropertyName,
                                  const QVariant& value,
                                  const QVariant& defaultValue);
    void getPropertiesFromObject(const QObject* object,
                                 QMap<QString, QString>* propertiesMap,
                                 QMap<QString, QVariant>* defaultValuesMap);

private:
    QObject* m_object = nullptr;
    Flexbox* m_flexbox = nullptr;
    QString m_className;

    const QMap<QString, QString>* qmlProperties();
    const QMap<QString, QString>* flexboxProperties();
    const QMap<QString, QVariant>* defaultQmlValues();
    const QMap<QString, QVariant>* defaultFlexboxValues();
    static QMap<QString, Props*> m_propsForClass;

    SetPropertyCallback m_setPropertyCallback;
};

#endif // PROPERTYHANDLER_H
