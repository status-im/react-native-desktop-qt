
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

#include <QMap>
#include <QObject>
#include <QMetaProperty>


class QQuickItem;


class ReactPropertyHandler : public QObject
{
  Q_OBJECT

public:
  ReactPropertyHandler(QObject* object, bool exposeQmlProperties = true);
  virtual ~ReactPropertyHandler();

  virtual QMap<QString, QMetaProperty> availableProperties();
  virtual void applyProperties(const QVariantMap& properties);
  QVariant value(const QString& propertyName);

protected:
  virtual void buildPropertyMap();
  void setValueToObjectProperty(QObject* object, QMetaProperty property, const QVariant& value);

protected:
  bool m_exposeQmlProperties;
  bool m_cached = false;
  QObject* m_object;
  QMap<QString, QMetaProperty> m_qmlProperties;
  QMap<QString, QMetaProperty> m_HandlerProperties;

};

#endif // REACTPROPERTYHANDLER_H
