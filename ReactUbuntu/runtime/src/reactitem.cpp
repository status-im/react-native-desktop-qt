
#include "reactitem.h"
#include "reactflexlayout.h"
#include "reacttextproperties.h"


class ReactItemPrivate
{
  Q_DECLARE_PUBLIC(ReactItem)
public:
  ReactItemPrivate(ReactItem* q)
    : q_ptr(q)
  {
  }

  ReactItem* q_ptr;
};

ReactItem::ReactItem(QQuickItem *parent)
  : QQuickItem(parent)
  , d_ptr(new ReactItemPrivate(this))
{
}

ReactItem::~ReactItem()
{
}

void ReactItem::updatePolish()
{
  ReactFlexLayout::get(this)->setWidth(width());
  ReactFlexLayout::get(this)->setHeight(height());

  ReactFlexLayout::polish(this);
}
