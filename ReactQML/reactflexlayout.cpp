#include <QQuickItem>

extern "C" {
#include "Layout.h"
}
#include "reactflexlayout.h"

QDebug operator<<(QDebug debug, const ReactFlexLayoutPrivate* p);

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
  }
  ~ReactFlexLayoutPrivate() {
    free_css_node(cssNode);
  }
  void setChildInfo(ReactFlexLayoutPrivate* p) {
    if (p->item == nullptr)
      return;
    p->cssNode->children_count = p->item->childItems().size();
    for (auto c : p->item->childItems()) {
      setChildInfo(ReactFlexLayoutPrivate::get(ReactFlexLayout::get(c)));
    }
  }
  void layout() {
    qDebug() << __PRETTY_FUNCTION__ << this;
    setChildInfo(this);
    print_css_node(cssNode, (css_print_options_t)(CSS_PRINT_STYLE | CSS_PRINT_CHILDREN));
    layoutNode(cssNode, CSS_UNDEFINED, CSS_DIRECTION_INHERIT);
    applyLayout();
  }
  static ReactFlexLayoutPrivate* get(ReactFlexLayout* rfl) {
    return rfl->d_func();
  }
  static css_node_t* getChild(void* context, int i) {
    QQuickItem* child =
      static_cast<ReactFlexLayoutPrivate*>(context)->item->childItems().at(i);
    return ReactFlexLayoutPrivate::get(ReactFlexLayout::get(child))->cssNode;
  }
  static bool isDirty(void* context) {
    static_cast<ReactFlexLayoutPrivate*>(context)->dirty;
  }
  bool dirty;
  QQuickItem* item;
  css_node_t* cssNode;
  ReactFlexLayout* q_ptr;

private:
  void applyLayout() {
    qDebug() << __PRETTY_FUNCTION__ << this;

    if (!cssNode->layout.should_update)
      return;
    cssNode->layout.should_update = false;

    item->setX(cssNode->layout.position[CSS_LEFT]);
    item->setY(cssNode->layout.position[CSS_TOP]);
    item->setWidth(cssNode->layout.dimensions[CSS_WIDTH]);
    item->setHeight(cssNode->layout.dimensions[CSS_HEIGHT]);

    for (QQuickItem* c : item->childItems()) {
      ReactFlexLayoutPrivate::get(ReactFlexLayout::get(c))->applyLayout();
    }
  }
};

QDebug operator<<(QDebug debug, const ReactFlexLayoutPrivate* p)
{
  QDebugStateSaver s(debug);
  debug.nospace() << "ReactFlexLayoutPrivate(this=" << (void*)p <<
    ", item=" << p->item << ", layout={" <<
    p->cssNode->layout.position[CSS_LEFT] << " " <<
    p->cssNode->layout.position[CSS_TOP] << " " <<
    p->cssNode->layout.dimensions[CSS_WIDTH] << " " <<
    p->cssNode->layout.dimensions[CSS_HEIGHT] << "})";
  return debug;
}

ReactFlexLayout::ReactFlexLayout(QObject* parent)
  : QObject(parent)
  , d_ptr(new ReactFlexLayoutPrivate(this))
{
  Q_D(ReactFlexLayout);
  d->dirty = true;
  d->item = qobject_cast<QQuickItem*>(parent);
  if (d->item == nullptr) {
    qCritical() << "Flex layout only applies to visual items";
  }
}

ReactFlexLayout::~ReactFlexLayout()
{
}

double ReactFlexLayout::flex() const
{
  Q_D(const ReactFlexLayout);
  return d->cssNode->style.flex;
}

void ReactFlexLayout::setFlex(double flex)
{
  Q_D(ReactFlexLayout);
  if (d->cssNode->style.flex == flex) // XXX:
    return;

  d->cssNode->style.flex = flex;
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

double ReactFlexLayout::top() const
{
  return d_func()->cssNode->style.position[CSS_TOP];
}

void ReactFlexLayout::setTop(double top)
{
  Q_D(ReactFlexLayout);
  if (d->cssNode->style.position[CSS_TOP] == top)
    return;

  d->cssNode->style.position[CSS_TOP] = top;
  Q_EMIT topChanged();
}

double ReactFlexLayout::right() const
{
  return d_func()->cssNode->style.position[CSS_RIGHT];
}

void ReactFlexLayout::setRight(double right)
{
  Q_D(ReactFlexLayout);
  if (d->cssNode->style.position[CSS_RIGHT] == right)
    return;

  d->cssNode->style.position[CSS_RIGHT] = right;
  Q_EMIT rightChanged();
}

double ReactFlexLayout::bottom() const
{
  return d_func()->cssNode->style.position[CSS_BOTTOM];
}

void ReactFlexLayout::setBottom(double bottom)
{
  Q_D(ReactFlexLayout);
  if (d->cssNode->style.position[CSS_BOTTOM] == bottom)
    return;

  d->cssNode->style.position[CSS_BOTTOM] = bottom;
  Q_EMIT bottomChanged();
}

double ReactFlexLayout::left() const
{
  return d_func()->cssNode->style.position[CSS_LEFT];
}

void ReactFlexLayout::setLeft(double left)
{
  Q_D(ReactFlexLayout);
  if (d->cssNode->style.position[CSS_LEFT] == left)
    return;

  d->cssNode->style.position[CSS_LEFT] = left;
  Q_EMIT leftChanged();
}

double ReactFlexLayout::width() const
{
  return d_func()->cssNode->style.dimensions[CSS_WIDTH];
}

void ReactFlexLayout::setWidth(double width)
{
  Q_D(ReactFlexLayout);
  if (d->cssNode->style.dimensions[CSS_WIDTH] == width)
    return;

  d->cssNode->style.dimensions[CSS_WIDTH] = width;
  Q_EMIT widthChanged();
}

double ReactFlexLayout::height() const
{
  return d_func()->cssNode->style.dimensions[CSS_HEIGHT];
}

void ReactFlexLayout::setHeight(double height)
{
  Q_D(ReactFlexLayout);
  if (d->cssNode->style.dimensions[CSS_HEIGHT] == height)
    return;

  d->cssNode->style.dimensions[CSS_HEIGHT] = height;
  Q_EMIT heightChanged();
}

double ReactFlexLayout::padding() const
{
  return d_func()->cssNode->style.padding[CSS_TOP]; // XXX: need to map this, like react?
}

void ReactFlexLayout::setPadding(double padding)
{
  Q_D(ReactFlexLayout);
  d->cssNode->style.padding[CSS_TOP] =
    d->cssNode->style.padding[CSS_LEFT] =
    d->cssNode->style.padding[CSS_BOTTOM] =
    d->cssNode->style.padding[CSS_RIGHT] = padding;
}

double ReactFlexLayout::paddingVertical() const
{
}

void ReactFlexLayout::setPaddingVertical(double padding)
{
}

double ReactFlexLayout::paddingHorizontal() const
{
}

void ReactFlexLayout::setPaddingHorizontal(double padding)
{
}

double ReactFlexLayout::paddingTop() const
{
}

void ReactFlexLayout::setPaddingTop(double padding)
{
}

double ReactFlexLayout::paddingLeft() const
{
}

void ReactFlexLayout::setPaddingLeft(double padding)
{
}

double ReactFlexLayout::paddingBottom() const
{
}

void ReactFlexLayout::setPaddingBottom(double padding)
{
}

double ReactFlexLayout::margin() const
{
  return d_func()->cssNode->style.margin[CSS_TOP]; // XXX: as with padding
}

void ReactFlexLayout::setMargin(double margin)
{
  Q_D(ReactFlexLayout);
  d->cssNode->style.padding[CSS_TOP] =
    d->cssNode->style.padding[CSS_LEFT] =
    d->cssNode->style.padding[CSS_BOTTOM] =
    d->cssNode->style.padding[CSS_RIGHT] = margin;
}

double ReactFlexLayout::marginBottom() const
{
  return d_func()->cssNode->style.margin[CSS_BOTTOM]; // XXX: as with padding
}

void ReactFlexLayout::setMarginBottom(double margin)
{
  Q_D(ReactFlexLayout);
  d->cssNode->style.margin[CSS_BOTTOM] = margin;
}

void ReactFlexLayout::layout()
{
  d_func()->layout();
}

ReactFlexLayout* ReactFlexLayout::get(QQuickItem* item, bool create)
{
  return qobject_cast<ReactFlexLayout*>(qmlAttachedPropertiesObject<ReactFlexLayout>(item, create));
}

ReactFlexLayout* ReactFlexLayout::qmlAttachedProperties(QObject* object)
{
  return new ReactFlexLayout(object);
}
