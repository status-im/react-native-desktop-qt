
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
#include <QQmlComponent>
#include <QQuickItem>
#include <QQmlProperty>

#include <QDebug>

#include "ubuntupagemanager.h"
#include "reactbridge.h"
#include "reactuimanager.h"
#include "reactpropertyhandler.h"
#include "reactflexlayout.h"


class PagePropertyHandler : public ReactPropertyHandler {
  Q_OBJECT
  Q_PROPERTY(QString title READ title WRITE setTitle)
  Q_PROPERTY(bool qmlAnchors READ qmlAnchors WRITE setQmlAnchors)
public:
  PagePropertyHandler(QObject* object)
    : ReactPropertyHandler(object) {
    }
  QString title() const;
  void setTitle(const QString& title);
  bool qmlAnchors() const;
  void setQmlAnchors(bool qmlAnchors);
};

QString PagePropertyHandler::title() const
{
  QQmlProperty(m_object, "title").read().value<QString>();
}

void PagePropertyHandler::setTitle(const QString& title)
{
  QQmlProperty(m_object, "title").write(QVariant::fromValue(title));
}

bool PagePropertyHandler::qmlAnchors() const
{
  return ReactFlexLayout::get(qobject_cast<QQuickItem*>(m_object))->qmlAnchors();
}

void PagePropertyHandler::setQmlAnchors(bool qmlAnchors)
{
  ReactFlexLayout::get(qobject_cast<QQuickItem*>(m_object))->setQmlAnchors(qmlAnchors);
}


UbuntuPageManager::UbuntuPageManager(QObject* parent)
  : ReactViewManager(parent)
  , m_id(0)
{
}

UbuntuPageManager::~UbuntuPageManager()
{
}

void UbuntuPageManager::setBridge(ReactBridge* bridge)
{
  m_bridge = bridge;
}

ReactViewManager* UbuntuPageManager::viewManager()
{
  return this;
}

ReactPropertyHandler* UbuntuPageManager::propertyHandler(QObject* object)
{
  Q_ASSERT(qobject_cast<QQuickItem*>(object) != nullptr);
  return new PagePropertyHandler(object);
}

QString UbuntuPageManager::moduleName()
{
  return "UbuntuPageManager";
}

QList<ReactModuleMethod*> UbuntuPageManager::methodsToExport()
{
  return QList<ReactModuleMethod*>{};
}

QVariantMap UbuntuPageManager::constantsToExport()
{
  return QVariantMap{};
}

namespace {
static const char* component_qml = R"COMPONENT(
import QtQuick 2.4
import Ubuntu.Components 1.2

Page {
  id: page%1
  visible: false
}
)COMPONENT";
}

QQuickItem* UbuntuPageManager::view(const QVariantMap& properties) const
{
  QString componentString = QString(component_qml).arg(m_id++);

  QQmlComponent component(m_bridge->qmlEngine());
  component.setData(componentString.toLocal8Bit(), QUrl());
  if (!component.isReady())
    qCritical() << "Component for UbuntuPageManager not ready" << component.errors();

  QQuickItem* item = qobject_cast<QQuickItem*>(component.create());
  if (item == nullptr) {
    qCritical() << "Unable to create component for UbuntuPageManager";
    return nullptr;
  }

  configureView(item);

  return item;
}

void UbuntuPageManager::widthChanged()
{
  QQuickItem* item = qobject_cast<QQuickItem*>(sender());
  QTimer::singleShot(0, [=] {
    ReactFlexLayout::get(item)->setDirty(true);
    m_bridge->visualParent()->polish();
  });
}

void UbuntuPageManager::heightChanged()
{
  QQuickItem* item = qobject_cast<QQuickItem*>(sender());
  QTimer::singleShot(0, [=] {
    ReactFlexLayout::get(item)->setDirty(true);
    m_bridge->visualParent()->polish();
  });
}

void UbuntuPageManager::configureView(QQuickItem* view) const
{
  connect(view, SIGNAL(widthChanged()), SLOT(widthChanged()));
  connect(view, SIGNAL(heightChanged()), SLOT(heightChanged()));
}

#include "ubuntupagemanager.moc"
