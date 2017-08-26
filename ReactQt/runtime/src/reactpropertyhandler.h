
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

  virtual QList<QMetaProperty> availableProperties();
  virtual void applyProperties(const QVariantMap& properties);

protected:
  bool m_exposeQmlProperties;
  bool m_cached = false;
  QObject* m_object;
  QMap<QString, QMetaProperty> m_coreProperties;
  QMap<QString, QMetaProperty> m_extraProperties;

private:
  void buildPropertyMap();
  void setValueToObjectProperty(QObject* object, QMetaProperty property, const QVariant& value);
};

#endif // REACTPROPERTYHANDLER_H
