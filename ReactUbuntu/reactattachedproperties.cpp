
#include <QQuickItem>

#include "reactattachedproperties.h"


class ReactAttachedPropertiesPrivate
{
public:
  ReactAttachedPropertiesPrivate()
    : tag(-1) {}
  int tag;
  QQuickItem* item;
};

ReactAttachedProperties::ReactAttachedProperties(QObject* parent)
  : QObject(parent)
  , d_ptr(new ReactAttachedPropertiesPrivate)
{
  Q_D(ReactAttachedProperties);
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
  Q_EMIT tagChanged();
}

ReactAttachedProperties* ReactAttachedProperties::get(QQuickItem* item, bool create)
{
  return qobject_cast<ReactAttachedProperties*>(qmlAttachedPropertiesObject<ReactAttachedProperties>(item, create));
}

ReactAttachedProperties* ReactAttachedProperties::qmlAttachedProperties(QObject* object)
{
  return new ReactAttachedProperties(object);
}
