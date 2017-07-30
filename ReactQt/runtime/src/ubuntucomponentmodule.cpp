
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

#include <QDebug>

#include "ubuntucomponentmodule.h"
#include "reactmodulemethod.h"
#include "reactbridge.h"
#include "reactuimanager.h"


UbuntuComponentModule::UbuntuComponentModule
(
 const QString& name,
 const QString& version,
 const QString& path
)
  : ReactViewManager(0)
  , m_name(name)
  , m_version(version)
  , m_path(path)
  , m_methodsExported(false)
{
}

UbuntuComponentModule::~UbuntuComponentModule()
{
}

void UbuntuComponentModule::setBridge(ReactBridge* bridge)
{
  ReactViewManager::setBridge(bridge);

}

ReactViewManager* UbuntuComponentModule::viewManager()
{
  return this;
}

QString UbuntuComponentModule::moduleName()
{
  return "Ubuntu" + m_name + "Manager";
}

QList<ReactModuleMethod*> UbuntuComponentModule::methodsToExport()
{
  // qDebug() << __PRETTY_FUNCTION__;
  if (!m_methodsExported) {
    m_methodsExported = true;

    // XXX: this is a bit wasteful, needs to be fixed, a global cache
    QQuickItem* v = view(QVariantMap{});
    if (v == nullptr) {
      qWarning() << __PRETTY_FUNCTION__ << "Could not allocate view for filling method cache!";
      return m_methodCache;
    }
    v->deleteLater();

    const QMetaObject* metaObject = v->metaObject();
    const int methodCount = metaObject->methodCount();

    for (int i = metaObject->methodOffset(); i < methodCount; ++i) {
      QMetaMethod m = metaObject->method(i);
      if (m.name().startsWith("__") || m.name().endsWith("Changed"))
        continue;
      m_methodCache << new ReactModuleMethod(
                              m,
                              [this](QVariantList& args) {
                                  // The first is the object to apply the function to
                                  if (args.length() == 0)
                                    return (QObject*)nullptr;
                                  Q_ASSERT(args.first().canConvert<int>());
                                  int reactTag = args.first().toInt();
                                  args.pop_front();

                                  QObject* module = m_bridge->uiManager()->viewForTag(reactTag);
                                  Q_ASSERT(module != nullptr);

                                  // XXX: Scan for objects with "reactTag"
                                  // These need to be converted to views
                                  for (int i = 0; i < args.size(); ++i) {
                                    if (args[i].type() == QVariant::Map) {
                                      QVariantMap m = args[i].toMap();
                                      if (m.size() == 1 && m.contains("reactTag")) {
                                        QQuickItem* item = m_bridge->uiManager()->viewForTag(m.value("reactTag").toInt());
                                        if (item == nullptr) {
                                          qCritical() << "Could not find view for tag" << m.value("reactTag");
                                          return (QObject*)nullptr;
                                        }
                                        args[i] = QVariant::fromValue(item);
                                      }
                                    }
                                  }
                                  return (QObject*)module;
                                });
    }
  }

  return m_methodCache;
}

QVariantMap UbuntuComponentModule::constantsToExport()
{
  return QVariantMap{};
}

QQuickItem* UbuntuComponentModule::view(const QVariantMap& properties) const
{
  // qDebug() << __PRETTY_FUNCTION__ << "begin";
  // Should probably build a string with import statement (using version)
  // instead of doing this
  //  QQmlComponent component(m_bridge->qmlEngine(), m_path, QQmlComponent::PreferSynchronous);
  QQmlComponent component(m_bridge->qmlEngine());
  QByteArray data = QString("import Ubuntu.Components %1\n%2{}").arg(m_version).arg(m_name).toLocal8Bit();
  component.setData(data, QUrl());
  if (!component.isReady())
    qCritical() << m_name << "was not ready!";

  QQuickItem* item = qobject_cast<QQuickItem*>(component.create());
  if (item == nullptr) {
    qCritical() << "Unable to construct" << m_name;
    return nullptr;
  }

  return item;
}

// void UbuntuComponentModule::applyProperties(QQuickItem* item, const QVariantMap& properties) const
// {
//   if (properties.isEmpty())
//     return;
// 
//   ReactViewManager::applyProperties(item, properties);
// }
