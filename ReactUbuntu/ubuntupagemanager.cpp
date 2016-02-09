
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

void PagePropertyHandler::setTitle(const QString& title)
{
  QQmlProperty p(m_object, "title");
  p.write(QVariant::fromValue(title));
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
