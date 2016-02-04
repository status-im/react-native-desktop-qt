
#include <QQuickItem>

#include "reactviewmanager.h"
#include "reactpropertyhandler.h"
#include "reactattachedproperties.h"


class ReactAttachedPropertiesPrivate
{
public:
  ReactAttachedPropertiesPrivate()
    : tag(-1) {}
  int tag;
  bool shouldLayout;
  ReactViewManager* viewManager;
  ReactPropertyHandler* propertyHandler;
  QQuickItem* item;
};

ReactAttachedProperties::ReactAttachedProperties(QObject* parent)
  : QObject(parent)
  , d_ptr(new ReactAttachedPropertiesPrivate)
{
  Q_D(ReactAttachedProperties);
  d->viewManager = nullptr;
  d->item = qobject_cast<QQuickItem*>(parent);
  if (d->item == nullptr) {
    qCritical() << "Flex layout only applies to visual items";
  }
}

ReactAttachedProperties::~ReactAttachedProperties()
{
}

int ReactAttachedProperties::tag() const
{
  return d_func()->tag;
}

void ReactAttachedProperties::setTag(int tag)
{
  Q_D(ReactAttachedProperties);
  if (d->tag == tag)
    return;
  d->tag = tag;
}

bool ReactAttachedProperties::shouldLayout() const
{
  return d_func()->shouldLayout;
}

void ReactAttachedProperties::setShouldLayout(bool shouldLayout)
{
  Q_D(ReactAttachedProperties);
  if (d->shouldLayout == shouldLayout)
    return;
  d->shouldLayout = shouldLayout;
}

ReactViewManager* ReactAttachedProperties::viewManager() const
{
  return d_func()->viewManager;
}

void ReactAttachedProperties::setViewManager(ReactViewManager* viewManager)
{
  Q_D(ReactAttachedProperties);
  if (d->viewManager == viewManager)
    return;
  d->viewManager = viewManager;
}

ReactPropertyHandler* ReactAttachedProperties::propertyHandler() const
{
  return d_func()->propertyHandler;
}

void ReactAttachedProperties::setPropertyHandler(ReactPropertyHandler* propertyHandler)
{
  Q_D(ReactAttachedProperties);
  if (d->propertyHandler == propertyHandler)
    return;
  d->propertyHandler = propertyHandler;
}

void ReactAttachedProperties::applyProperties(const QVariantMap& properties) const
{
  Q_D(const ReactAttachedProperties);
  Q_ASSERT(d->propertyHandler != nullptr);
  d->propertyHandler->applyProperties(properties);
}

ReactAttachedProperties* ReactAttachedProperties::get(QQuickItem* item, bool create)
{
  return qobject_cast<ReactAttachedProperties*>(qmlAttachedPropertiesObject<ReactAttachedProperties>(item, create));
}

ReactAttachedProperties* ReactAttachedProperties::qmlAttachedProperties(QObject* object)
{
  return new ReactAttachedProperties(object);
}
