
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
#include "reacttextproperties.h"
#include "qmlpropertyhandler.h"


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
  return new QmlPropertyHandler(object);
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
}

QQuickItem* ReactViewManager::view(const QVariantMap& properties) const
{
  QQuickItem* item = createViewFromFile(":/qml/ReactView.qml");
  if(item)
  {
    item->setProperty("imageManager", QVariant::fromValue((QObject*)this));
  }
  return item;
}


QQuickItem*ReactViewManager::createViewFromFile(const QString& fileName) const
{
  QQmlComponent component(m_bridge->qmlEngine());
  component.loadUrl(QUrl::fromLocalFile(fileName));
  if (!component.isReady())
  {
    qCritical() << QString("Component for %1 is not ready!").arg(fileName) << component.errors();
    return nullptr;
  }

  QQuickItem* item = qobject_cast<QQuickItem*>(component.create());
  if (item == nullptr) {
    qCritical() << QString("Unable to construct item from component %1").arg(fileName);
  }
  return item;
}

ReactBridge*ReactViewManager::bridge()
{
  Q_ASSERT(m_bridge);
  return m_bridge;
}


void ReactViewManager::manageTransformMatrix(const QVector<float>& transformMatrix, QQuickItem* object)
{
  QQmlListReference r(object, "transform");
  r.clear();
  r.append(new MatrixTransform(transformMatrix, qobject_cast<QQuickItem*>(object)));
}


#include "reactviewmanager.moc"
