
#include <QString>
#include <QVariant>
#include <QQmlComponent>
#include <QQuickItem>

#include <QDebug>

#include "reactattachedproperties.h"
#include "reactevents.h"
#include "ubuntutextfieldmanager.h"
#include "reactbridge.h"
#include "reactpropertyhandler.h"
#include "reactevents.h"


class TextFieldPropertyHandler : public ReactPropertyHandler {
  Q_OBJECT
  Q_PROPERTY(bool onFocus READ onFocus WRITE setOnFocus)
  Q_PROPERTY(bool onChange READ onChange WRITE setOnChange)
public:
  TextFieldPropertyHandler(QObject* object)
    : ReactPropertyHandler(object) {
    }
  bool onFocus() const;
  void setOnFocus(bool onFocus);
  bool m_onFocus;

  bool onChange() const;
  void setOnChange(bool onChanged);
  bool m_onChange;
};

bool TextFieldPropertyHandler::onFocus() const
{
  return m_onFocus;
}

void TextFieldPropertyHandler::setOnFocus(bool onFocus)
{
  m_onFocus = onFocus;
}

bool TextFieldPropertyHandler::onChange() const
{
  return m_onChange;
}

void TextFieldPropertyHandler::setOnChange(bool onChange)
{
  m_onChange = onChange;
}


UbuntuTextFieldManager::UbuntuTextFieldManager(QObject* parent)
  : ReactViewManager(parent)
{
}

UbuntuTextFieldManager::~UbuntuTextFieldManager()
{
}

void UbuntuTextFieldManager::setBridge(ReactBridge* bridge)
{
  m_bridge = bridge;
}

ReactViewManager* UbuntuTextFieldManager::viewManager()
{
  return this;
}

ReactPropertyHandler* UbuntuTextFieldManager::propertyHandler(QObject* object)
{
  Q_ASSERT(qobject_cast<QQuickItem*>(object) != nullptr);
  return new TextFieldPropertyHandler(object);
}

QString UbuntuTextFieldManager::moduleName()
{
  return "UbuntuTextFieldManager";
}

QList<ReactModuleMethod*> UbuntuTextFieldManager::methodsToExport()
{
  return QList<ReactModuleMethod*>{};
}

QVariantMap UbuntuTextFieldManager::constantsToExport()
{
  return QVariantMap{};
}

QStringList UbuntuTextFieldManager::customDirectEventTypes()
{
  return QStringList{"scrollBeginDrag", normalizeInputEventName("onScroll"), "scrollEndDrag", "scrollAnimationEnd",
                     "momentumScrollBegin", "momentumScrollEnd"};
}

namespace {
static const char* component_qml =
"import QtQuick 2.4\n"
"import Ubuntu.Components 1.2\n"
"\n"
"TextField {\n"
"}\n";
}

QQuickItem* UbuntuTextFieldManager::view(const QVariantMap& properties) const
{
  qDebug() << __PRETTY_FUNCTION__ << properties;

  QQmlComponent component(m_bridge->qmlEngine());
  component.setData(component_qml, QUrl());
  if (!component.isReady())
    qCritical() << "Component for UbuntuScrollView not ready";

  QQuickItem* item = qobject_cast<QQuickItem*>(component.create());
  if (item == nullptr) {
    qCritical() << "Unable to create component for UbuntuTextFieldManager";
    return nullptr;
  }

  configureView(item);

  return item;
}

namespace {
template<typename TP>
TP propertyValue(QQuickItem* item, const QString& property)
{
  return QQmlProperty(item, property).read().value<TP>();
}

QVariantMap buildEventData(QQuickItem* item)
{
  static int eventCount = 0;

  eventCount++; // XXX:

  return QVariantMap{
    { "text", propertyValue<QString>(item, "text") },
    { "eventCount", eventCount },
  };
}
}

void UbuntuTextFieldManager::activeFocusChanged()
{
  QQuickItem* item = qobject_cast<QQuickItem*>(sender());
  Q_ASSERT(item != nullptr);

  ReactAttachedProperties* ap = ReactAttachedProperties::get(item);
  if (ap == nullptr) {
    qCritical() << __PRETTY_FUNCTION__ << "failed to find ReactAttachedProperties";
    return;
  }
  TextFieldPropertyHandler* ph = qobject_cast<TextFieldPropertyHandler*>(ap->propertyHandler());
  if (ph == nullptr) {
    qCritical() << __PRETTY_FUNCTION__ << "failed to find TextFieldPropertyHandler";
    return;
  }
  if (ph->onFocus()) {
    m_bridge->enqueueJSCall("RCTEventEmitter", "receiveEvent",
                            QVariantList{ap->tag(),
                                         normalizeInputEventName("onFocus"),
                                         QVariantMap{}});
  }
}

void UbuntuTextFieldManager::textChanged()
{
  qDebug() << __PRETTY_FUNCTION__;
  QQuickItem* item = qobject_cast<QQuickItem*>(sender());
  Q_ASSERT(item != nullptr);

  ReactAttachedProperties* ap = ReactAttachedProperties::get(item);
  if (ap == nullptr) {
    qCritical() << __PRETTY_FUNCTION__ << "failed to find ReactAttachedProperties";
    return;
  }
  TextFieldPropertyHandler* ph = qobject_cast<TextFieldPropertyHandler*>(ap->propertyHandler());
  if (ph == nullptr) {
    qCritical() << __PRETTY_FUNCTION__ << "failed to find TextFieldPropertyHandler";
    return;
  }
  if (ph->onChange()) {
    qDebug() << __PRETTY_FUNCTION__ << "sending message";
    m_bridge->enqueueJSCall("RCTEventEmitter", "receiveEvent",
                            QVariantList{ap->tag(),
                                         normalizeInputEventName("onChange"),
                                         buildEventData(item)});
  }
}

void UbuntuTextFieldManager::configureView(QQuickItem* view) const
{
  // This would be prettier with a Functor version, but connect doesnt support it
  connect(view, SIGNAL(activeFocusChanged()), SLOT(activeFocusChanged()));
  connect(view, SIGNAL(textChanged()), SLOT(textChanged()));
}

#include "ubuntutextfieldmanager.moc"
