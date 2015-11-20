#include <QQuickItem>

extern "C" {
#include "Layout.h"
}
#include "reactflexlayout.h"

class ReactFlexLayoutPrivate
{
  Q_DECLARE_PUBLIC(ReactFlexLayout)
public:
  ReactFlexLayoutPrivate(ReactFlexLayout* q)
    : q_ptr(q)
    , cssNode(new_css_node()) {
    cssNode->context = this;
    cssNode->get_child = ReactFlexLayoutPrivate::getChild;
    cssNode->is_dirty = ReactFlexLayoutPrivate::isDirty;
    setChildInfo();
  }
  ~ReactFlexLayoutPrivate() {
    free_css_node(cssNode);
  }
  void setChildInfo() {
    if (item == nullptr) {
      cssNode->children_count = 0;
      return;
    }
    cssNode->children_count = item->childItems().size();
  }
  void layout() {
    setChildInfo();
    layoutNode(cssNode, CSS_UNDEFINED, CSS_DIRECTION_INHERIT);
  }
  static ReactFlexLayoutPrivate* get(ReactFlexLayout* rfl) {
    return rfl->d_func();
  }
  static css_node_t* getChild(void* context, int i) {
    QQuickItem* child =
      static_cast<ReactFlexLayoutPrivate*>(context)->item->childItems().at(i);
    ReactFlexLayout* flex = ReactFlexLayout::get(child);
    return ReactFlexLayoutPrivate::get(flex)->cssNode;
  }
  static bool isDirty(void* context) {
    static_cast<ReactFlexLayoutPrivate*>(context)->dirty;
  }
  bool dirty;
  QQuickItem* item;
  css_node_t* cssNode;
  ReactFlexLayout* q_ptr;
};

ReactFlexLayout::ReactFlexLayout(QObject* parent)
  : QObject(parent)
  , d_ptr(new ReactFlexLayoutPrivate(this))
{
  Q_D(ReactFlexLayout);
  d->dirty = false;
  d->item = qobject_cast<QQuickItem*>(parent);
  if (d->item == nullptr) {
    qCritical() << "Flex layout only applies to visual items";
  }
}

ReactFlexLayout::~ReactFlexLayout()
{
}

bool ReactFlexLayout::isFlex() const
{
  Q_D(const ReactFlexLayout);
  return d->item != nullptr && d->cssNode->style.flex > 0;
}

void ReactFlexLayout::setFlex(bool flex)
{
  Q_D(ReactFlexLayout);
  if ((d->cssNode->style.flex > 0) == flex)
    return;

  d->cssNode->style.flex = flex ? 1.0 : 0.0;
  Q_EMIT flexChanged();
}

ReactFlexLayout::Direction ReactFlexLayout::direction() const
{
  return (ReactFlexLayout::Direction)d_func()->cssNode->style.flex_direction;
}

void ReactFlexLayout::setDirection(Direction direction)
{
  Q_D(ReactFlexLayout);
  if (d->cssNode->style.flex_direction == (css_flex_direction_t)direction)
    return;

  d->cssNode->style.flex_direction = (css_flex_direction_t)direction;
  Q_EMIT directionChanged();
}

ReactFlexLayout::Justify ReactFlexLayout::justify() const
{
  return (ReactFlexLayout::Justify)d_func()->cssNode->style.justify_content;
}

void ReactFlexLayout::setJustify(Justify justify)
{
  Q_D(ReactFlexLayout);
  if (d->cssNode->style.justify_content == (css_justify_t)justify)
    return;

  d->cssNode->style.justify_content = (css_justify_t)justify;
  Q_EMIT justifyChanged();
}

ReactFlexLayout::Alignment ReactFlexLayout::selfAlignment() const
{
  return (ReactFlexLayout::Alignment)d_func()->cssNode->style.align_self;
}

void ReactFlexLayout::setSelfAlignment(Alignment alignment)
{
  Q_D(ReactFlexLayout);
  if (d->cssNode->style.align_self == (css_align_t)alignment)
    return;

  d->cssNode->style.align_self = (css_align_t)alignment;
  Q_EMIT selfAlignmentChanged();
}

ReactFlexLayout::Alignment ReactFlexLayout::itemAlignment() const
{
  return (ReactFlexLayout::Alignment)d_func()->cssNode->style.align_items;
}

void ReactFlexLayout::setItemAlignment(Alignment alignment)
{
  Q_D(ReactFlexLayout);
  if (d->cssNode->style.align_items == (css_align_t)alignment)
    return;

  d->cssNode->style.align_items = (css_align_t)alignment;
  Q_EMIT itemAlignmentChanged();
}

ReactFlexLayout::Position ReactFlexLayout::position() const
{
  return (ReactFlexLayout::Position)d_func()->cssNode->style.position_type;
}

void ReactFlexLayout::setPosition(Position position)
{
  Q_D(ReactFlexLayout);
  if (d->cssNode->style.position_type == (css_position_type_t)position)
    return;

  d->cssNode->style.position_type = (css_position_type_t)position;
  Q_EMIT positionChanged();
}

ReactFlexLayout::Wrap ReactFlexLayout::wrap() const
{
  return (ReactFlexLayout::Wrap)d_func()->cssNode->style.flex_wrap;
}

void ReactFlexLayout::setWrap(Wrap wrap)
{
  Q_D(ReactFlexLayout);
  if (d->cssNode->style.flex_wrap == (css_wrap_type_t)wrap)
    return;

  d->cssNode->style.flex_wrap = (css_wrap_type_t)wrap;
  Q_EMIT wrapChanged();
}

void ReactFlexLayout::layout()
{
  d_func()->layout();
}

ReactFlexLayout* ReactFlexLayout::get(QQuickItem* item)
{
  return qobject_cast<ReactFlexLayout*>(qmlAttachedPropertiesObject<ReactFlexLayout>(item));
}

ReactFlexLayout* ReactFlexLayout::qmlAttachedProperties(QObject* object)
{
  return new ReactFlexLayout(object);
}
