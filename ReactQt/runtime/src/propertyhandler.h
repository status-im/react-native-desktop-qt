
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
        //        qDebug() << "props created";
        m_qmlProperties = new QMap<QString, QMetaProperty>();
        m_flexboxProperties = new QMap<QString, QMetaProperty>();
        m_defaultQmlValues = new QMap<QString, QVariant>();
        m_defaultFlexboxValues = new QMap<QString, QVariant>();
    };

    ~Props() {
        //        qDebug() << "props destroyed";
        delete m_qmlProperties;
        delete m_flexboxProperties;
        delete m_defaultQmlValues;
        delete m_defaultFlexboxValues;
    }

    QMap<QString, QMetaProperty>* m_qmlProperties = nullptr;
    QMap<QString, QMetaProperty>* m_flexboxProperties = nullptr;
    QMap<QString, QVariant>* m_defaultQmlValues = nullptr;
    QMap<QString, QVariant>* m_defaultFlexboxValues = nullptr;
};

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
                                 QMap<QString, QMetaProperty>* propertiesMap,
                                 QMap<QString, QVariant>* defaultValuesMap);

private:
    //    bool m_cached = false;
    QObject* m_object = nullptr;
    Flexbox* m_flexbox = nullptr;
    QString m_className;

    const QMap<QString, QMetaProperty>* qmlProperties();
    const QMap<QString, QMetaProperty>* flexboxProperties();
    const QMap<QString, QVariant>* defaultQmlValues();
    const QMap<QString, QVariant>* defaultFlexboxValues();
    static QMap<QString, Props*> m_propsForClass;
    static QMutex m_mutex;

    SetPropertyCallback m_setPropertyCallback;
};

#endif // PROPERTYHANDLER_H
