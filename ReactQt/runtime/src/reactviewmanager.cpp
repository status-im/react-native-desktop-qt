
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

#include <QString>
#include <QVariant>
#include <QQuickItem>
#include <QQmlProperty>
#include <QMatrix4x4>

#include "reactviewmanager.h"
#include "reactitem.h"
#include "reactbridge.h"
#include "reactvaluecoercion.h"
#include "reactflexlayout.h"
#include "reactpropertyhandler.h"
#include "reacttextproperties.h"


class MatrixTransform : public QQuickTransform {
  Q_OBJECT
public:
  MatrixTransform(const QVector<float>& transformMatrix, QQuickItem* parent)
    : QQuickTransform(parent)
    , m_item(qobject_cast<QQuickItem*>(parent))
  {
    memcpy(m_transformMatrix.data(), transformMatrix.constData(), 16 * sizeof(float));
    m_transformMatrix.optimize();
  }
  void applyTo(QMatrix4x4 *matrix) const override {
    if (m_transformMatrix.isIdentity())
      return;
    matrix->translate(m_item->width() / 2, m_item->height() / 2);
    *matrix *= m_transformMatrix;
    matrix->translate(-m_item->width() / 2, -m_item->height() / 2);
  }
  QMatrix4x4 m_transformMatrix;
  QQuickItem* m_item;
};

class ViewPropertyHandler : public ReactPropertyHandler {
  Q_OBJECT
  Q_PROPERTY(QVector<float> transformMatrix READ transformMatrix WRITE setTransformMatrix)
public:
  ViewPropertyHandler(QObject* object)
    : ReactPropertyHandler(object)
    {}
  QVector<float> transformMatrix() const {
    return QVector<float>{};
  }
  void setTransformMatrix(const QVector<float>& transformMatrix) {
    QQmlListReference r(m_object, "transform");
    r.clear();
    r.append(new MatrixTransform(transformMatrix, qobject_cast<QQuickItem*>(m_object)));
  }
};


ReactViewManager::ReactViewManager(QObject *parent)
  : QObject(parent)
{
}

ReactViewManager::~ReactViewManager()
{
}

void ReactViewManager::setBridge(ReactBridge* bridge)
{
  m_bridge = bridge;
}

// TODO: this doesnt seem right
ReactViewManager* ReactViewManager::viewManager()
{
  return this;
}

ReactPropertyHandler* ReactViewManager::propertyHandler(QObject* object)
{
  return new ViewPropertyHandler(object);
}

QString ReactViewManager::moduleName()
{
  return "RCTViewManager";
}

QList<ReactModuleMethod*> ReactViewManager::methodsToExport()
{
  return QList<ReactModuleMethod*>{};
}

QVariantMap ReactViewManager::constantsToExport()
{
  return QVariantMap{};
}

QStringList ReactViewManager::customDirectEventTypes()
{
  return QStringList{};
}

QStringList ReactViewManager::customBubblingEventTypes()
{
  return QStringList{"press", "change", "focus", "blur", "submitEditing", "endEditing", "touchStart", "touchMove", "touchCancel", "touchEnd"};
}

bool ReactViewManager::shouldLayout() const
{
  return true;
}

void ReactViewManager::addChildItem(QQuickItem* container, QQuickItem* child, int position) const
{
  // XXX: remove this
  if ((ReactTextProperties::get(container, false) == nullptr) &&
      (ReactTextProperties::get(child, false) != nullptr)) {
    ReactTextProperties::get(child)->hookLayout();
  }
  child->setParentItem(container);
  child->setProperty("index", position);
}

namespace {
static const char* component_qml = R"COMPONENT(
import QtQuick 2.4
import React 0.1 as React
React.Item {
}
)COMPONENT";
}

QQuickItem* ReactViewManager::view(const QVariantMap& properties) const
{
  QQmlComponent component(m_bridge->qmlEngine());
  component.setData(component_qml, QUrl());
  if (!component.isReady())
    qCritical() << "React.Item is not ready!" << component.errors();

  QQuickItem* item = qobject_cast<QQuickItem*>(component.create());
  if (item == nullptr) {
    qCritical() << "Unable to construct React.Item";
    return nullptr;
  }

  return item;
}


#include "reactviewmanager.moc"
