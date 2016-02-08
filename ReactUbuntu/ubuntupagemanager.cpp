
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


class PagePropertyHandler : public ReactPropertyHandler {
  Q_OBJECT
  Q_PROPERTY(QString title READ title WRITE setTitle)
public:
  PagePropertyHandler(QObject* object)
    : ReactPropertyHandler(object) {
    }
  QString title() const;
  void setTitle(const QString& title);
};

void PagePropertyHandler::setTitle(const QString& title)
{
  qDebug() << __PRETTY_FUNCTION__ << m_object << title;
  // m_object->setProperty("title", title);
  QQmlProperty p(m_object, "title");
  p.write(QVariant::fromValue(title));
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
static const char* component_qml =
"import QtQuick 2.4\n"
"import Ubuntu.Components 1.2\n"
"\n"
"Page {\n"
"  id: page%1\n"
"  visible: false\n"
"}\n";
}

QQuickItem* UbuntuPageManager::view(const QVariantMap& properties) const
{
  QString componentString = QString(component_qml).arg(m_id++);

  QQmlComponent component(m_bridge->qmlEngine());
  component.setData(componentString.toLocal8Bit(), QUrl());
  if (!component.isReady())
    qCritical() << "Component for UbuntuPageManager not ready";

  QQuickItem* item = qobject_cast<QQuickItem*>(component.create());
  if (item == nullptr) {
    qCritical() << "Unable to create component for UbuntuPageManager";
    return nullptr;
  }

  return item;
}

#include "ubuntupagemanager.moc"
