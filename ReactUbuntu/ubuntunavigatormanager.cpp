
#include <QString>
#include <QVariant>
#include <QQmlComponent>
#include <QQuickItem>

#include <QDebug>

#include "ubuntunavigatormanager.h"
#include "reactbridge.h"
#include "reactuimanager.h"

void UbuntuNavigatorManager::push(int containerTag, int viewTag)
{
  QQuickItem* container = m_bridge->uiManager()->viewForTag(containerTag);
  QQuickItem* view = m_bridge->uiManager()->viewForTag(viewTag);
  if (container == nullptr || view == nullptr) {
    qCritical() << __PRETTY_FUNCTION__ << "Invalid react tag passed";
    return;
  }

  invokeMethod("push(QVariant)", container, QVariantList{QVariant::fromValue(view)});
}

void UbuntuNavigatorManager::pop(int containerTag)
{
  QQuickItem* container = m_bridge->uiManager()->viewForTag(containerTag);
  if (container == nullptr) {
    qCritical() << __PRETTY_FUNCTION__ << "Invalid react tag passed";
    return;
  }
  invokeMethod("pop()", container);
}

void UbuntuNavigatorManager::clear(int containerTag)
{
  QQuickItem* container = m_bridge->uiManager()->viewForTag(containerTag);
  if (container == nullptr) {
    qCritical() << __PRETTY_FUNCTION__ << "Invalid react tag passed";
    return;
  }
  invokeMethod("clear()", container);
}


UbuntuNavigatorManager::UbuntuNavigatorManager(QObject* parent)
  : ReactViewManager(parent)
  , m_id(0)
{
}

UbuntuNavigatorManager::~UbuntuNavigatorManager()
{
}

void UbuntuNavigatorManager::setBridge(ReactBridge* bridge)
{
  m_bridge = bridge;
}

ReactViewManager* UbuntuNavigatorManager::viewManager()
{
  return this;
}

QString UbuntuNavigatorManager::moduleName()
{
  return "UbuntuNavigatorManager";
}

QList<ReactModuleMethod*> UbuntuNavigatorManager::methodsToExport()
{
  return QList<ReactModuleMethod*>{};
}

QVariantMap UbuntuNavigatorManager::constantsToExport()
{
  return QVariantMap{};
}

namespace {
static const char* component_qml =
"import QtQuick 2.4\n"
"import Ubuntu.Components 1.2\n"
"\n"
"MainView {\n"
"  id: mainView%1\n"
"  PageStack {\n"
"    id: pageStack%1\n"
"    anchors.fill: parent\n"
"  }\n"
"  function push(item) {\n"
"    pageStack%1.push(item);\n"
"  }\n"
"  function pop() {\n"
"    pageStack%1.pop();\n"
"  }\n"
"  function clear() {\n"
"    pageStack%1.clear();\n"
"  }\n"
"}\n";
}

QQuickItem* UbuntuNavigatorManager::view(const QVariantMap& properties) const
{
  qDebug() << __PRETTY_FUNCTION__ << properties;
  QString componentString = QString(component_qml).arg(m_id++);

  QQmlComponent component(m_bridge->qmlEngine());
  component.setData(componentString.toLocal8Bit(), QUrl());
  if (!component.isReady())
    qCritical() << "Component for UbuntuNavigatorManager not ready";

  QQuickItem* item = qobject_cast<QQuickItem*>(component.create());
  if (item == nullptr) {
    qCritical() << "Unable to create component for UbuntuNavigatorManager";
    return nullptr;
  }

  configureView(item);

  return item;
}

void UbuntuNavigatorManager::configureView(QQuickItem* view) const
{
}

#define _R_ARG(argn) QGenericArgument(argn.typeName(), argn.data())
void UbuntuNavigatorManager::invokeMethod
(
  const QString& methodSignature,
  QQuickItem* item,
  const QVariantList& args
) {
  QMetaMethod m = findMethod(methodSignature, item);
  if (!m.isValid()) {
    qCritical() << __PRETTY_FUNCTION__ << "Attempting to invoke unknown method";
    return;
  }

  // XXX: share this machinery somehow
  switch (args.size()) {
    case 0:
    m.invoke(item, Qt::DirectConnection);
    break;
  case 1:
    m.invoke(item, Qt::DirectConnection, _R_ARG(args.at(0)));
    break;
  }
}

QMetaMethod UbuntuNavigatorManager::findMethod
(
  const QString& methodSignature,
  QQuickItem* item
) {
  QPair<QString, QQuickItem*> mp = qMakePair(methodSignature, item);
  if (m_methodCache.find(mp) != m_methodCache.end()) {
    return m_methodCache.value(mp);
  } else {
    const QMetaObject* metaObject = item->metaObject();
    const int methodCount = metaObject->methodCount();

    for (int i = metaObject->methodOffset(); i < methodCount; ++i) {
      QMetaMethod m = metaObject->method(i);
      if (m.methodSignature() == methodSignature.toLocal8Bit()) {
        m_methodCache.insert(qMakePair(methodSignature, item), m);
        return m;
      }
    }
  }

  return QMetaMethod();
}
