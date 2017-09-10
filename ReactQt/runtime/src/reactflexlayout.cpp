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

#include <QQuickItem>

extern "C" {
#include "Layout.h"
}

#include "reactattachedproperties.h"
#include "reactflexlayout.h"
#include "reactvaluecoercion.h"

namespace {
static QMap<QString, ReactFlexLayout::Direction> directions{{"row", ReactFlexLayout::DirectionRow},
                                                            {"column", ReactFlexLayout::DirectionColumn}};
static QMap<QString, ReactFlexLayout::Alignment> alignments{{"flex-start", ReactFlexLayout::AlignmentFlexStart},
                                                            {"flex-end", ReactFlexLayout::AlignmentFlexEnd},
                                                            {"center", ReactFlexLayout::AlignmentCenter},
                                                            {"stretch", ReactFlexLayout::AlignmentStretch}};
static QMap<QString, ReactFlexLayout::Justify> justifys{{"flex-start", ReactFlexLayout::JustifyFlexStart},
                                                        {"flex-end", ReactFlexLayout::JustifyFlexEnd},
                                                        {"center", ReactFlexLayout::JustifyCenter},
                                                        {"space-between", ReactFlexLayout::JustifySpaceBetween},
                                                        {"space-around", ReactFlexLayout::JustifySpaceAround}};
static QMap<QString, ReactFlexLayout::Position> positions{{"absolute", ReactFlexLayout::PositionAbsolute},
                                                          {"relative", ReactFlexLayout::PositionRelative}};
static QMap<QString, ReactFlexLayout::Wrap> wraps{{"wrap", ReactFlexLayout::WrapYes},
                                                  {"nowrap", ReactFlexLayout::WrapNo}};
}

class FlexPropertyHandler {
public:
    FlexPropertyHandler() {
        const QMetaObject& metaObject = ReactFlexLayout::staticMetaObject;
        const int propertyCount = metaObject.propertyCount();

        for (int i = metaObject.propertyOffset(); i < propertyCount; ++i) {
            QMetaProperty p = metaObject.property(i);
            m_properties.insert(p.name(), p);
        }
    }

    void applyProperties(ReactFlexLayout* fl, const QVariantMap& properties) {
        for (auto it = properties.constBegin(); it != properties.constEnd(); ++it) {
            QMap<QString, QMetaProperty>::iterator pi = m_properties.find(it.key());
            if (pi != m_properties.end()) {
                pi.value().write(fl, reactCoerceValue(it.value(), pi.value().userType(), &m_coercions));
            }
        }
    }

    QMap<int, coerce_function> m_coercions{
        {qMetaTypeId<ReactFlexLayout::Direction>(),
         [](const QVariant& value) {
             Q_ASSERT(value.canConvert<QString>());
             return directions[value.toString()];
         }},
        {qMetaTypeId<ReactFlexLayout::Alignment>(),
         [](const QVariant& value) {
             Q_ASSERT(value.canConvert<QString>());
             return alignments[value.toString()];
         }},
        {qMetaTypeId<ReactFlexLayout::Justify>(),
         [](const QVariant& value) {
             Q_ASSERT(value.canConvert<QString>());
             return justifys[value.toString()];
         }},
        {qMetaTypeId<ReactFlexLayout::Position>(),
         [](const QVariant& value) {
             Q_ASSERT(value.canConvert<QString>());
             return positions[value.toString()];
         }},
        {qMetaTypeId<ReactFlexLayout::Wrap>(),
         [](const QVariant& value) {
             Q_ASSERT(value.canConvert<QString>());
             return wraps[value.toString()];
         }},
    };

    QMap<QString, QMetaProperty> m_properties;
};
Q_GLOBAL_STATIC(FlexPropertyHandler, flexPropertyHandler);

QDebug operator<<(QDebug debug, const ReactFlexLayoutPrivate* p);

namespace {
enum { CSS_HORIZONTAL = CSS_BOTTOM + 1, CSS_VERTICAL, CSS_PROP_COUNT };
}

class ReactFlexLayoutPrivate {
    Q_DECLARE_PUBLIC(ReactFlexLayout)
public:
    ReactFlexLayoutPrivate(ReactFlexLayout* q) : q_ptr(q), cssNode(new_css_node()) {
        cssNode->context = this;
        cssNode->get_child = ReactFlexLayoutPrivate::getChild;
        cssNode->is_dirty = ReactFlexLayoutPrivate::isDirty;
    }
    ~ReactFlexLayoutPrivate() {
        free_css_node(cssNode);
    }
    void updateMargin(ReactFlexLayoutPrivate* p) {
        if (!isUndefined(p->margin[CSS_LEFT])) {
            p->cssNode->style.margin[CSS_LEFT] = p->margin[CSS_LEFT];
        } else if (!isUndefined(p->margin[CSS_HORIZONTAL])) {
            p->cssNode->style.margin[CSS_LEFT] = p->margin[CSS_HORIZONTAL];
        }
        if (!isUndefined(p->margin[CSS_RIGHT])) {
            p->cssNode->style.margin[CSS_RIGHT] = p->margin[CSS_RIGHT];
        } else if (!isUndefined(p->margin[CSS_HORIZONTAL])) {
            p->cssNode->style.margin[CSS_RIGHT] = p->margin[CSS_HORIZONTAL];
        }

        if (!isUndefined(p->margin[CSS_TOP])) {
            p->cssNode->style.margin[CSS_TOP] = p->margin[CSS_TOP];
        } else if (!isUndefined(p->margin[CSS_VERTICAL])) {
            p->cssNode->style.margin[CSS_TOP] = p->margin[CSS_VERTICAL];
        }
        if (!isUndefined(p->margin[CSS_BOTTOM])) {
            p->cssNode->style.margin[CSS_BOTTOM] = p->margin[CSS_BOTTOM];
        } else if (!isUndefined(p->margin[CSS_VERTICAL])) {
            p->cssNode->style.margin[CSS_BOTTOM] = p->margin[CSS_VERTICAL];
        }
    }
    void updatePadding(ReactFlexLayoutPrivate* p) {
        if (!isUndefined(p->padding[CSS_LEFT])) {
            p->cssNode->style.padding[CSS_LEFT] = p->padding[CSS_LEFT];
        } else if (!isUndefined(p->padding[CSS_HORIZONTAL])) {
            p->cssNode->style.padding[CSS_LEFT] = p->padding[CSS_HORIZONTAL];
        }
        if (!isUndefined(p->padding[CSS_RIGHT])) {
            p->cssNode->style.padding[CSS_RIGHT] = p->padding[CSS_RIGHT];
        } else if (!isUndefined(p->padding[CSS_HORIZONTAL])) {
            p->cssNode->style.padding[CSS_RIGHT] = p->padding[CSS_HORIZONTAL];
        }

        if (!isUndefined(p->padding[CSS_TOP])) {
            p->cssNode->style.padding[CSS_TOP] = p->padding[CSS_TOP];
        } else if (!isUndefined(p->padding[CSS_VERTICAL])) {
            p->cssNode->style.padding[CSS_TOP] = p->padding[CSS_VERTICAL];
        }
        if (!isUndefined(p->padding[CSS_BOTTOM])) {
            p->cssNode->style.padding[CSS_BOTTOM] = p->padding[CSS_BOTTOM];
        } else if (!isUndefined(p->padding[CSS_VERTICAL])) {
            p->cssNode->style.padding[CSS_BOTTOM] = p->padding[CSS_VERTICAL];
        }
    }
    void prepareLayout(ReactFlexLayoutPrivate* p) {
        p->cssNode->children_count = p->children.size();
        updateMargin(p);
        updatePadding(p);

        if (p->qmlAnchors) {
            p->cssNode->style.dimensions[CSS_WIDTH] = p->item->width();
            p->cssNode->style.dimensions[CSS_HEIGHT] = p->item->height();
        }
        if (p->qmlImplicitWidth) {
            p->cssNode->style.dimensions[CSS_WIDTH] = QQmlProperty(p->item, "implictWidth").read().value<double>();
        }
        if (p->qmlImplicitHeight) {
            p->cssNode->style.dimensions[CSS_HEIGHT] = QQmlProperty(p->item, "implictHeight").read().value<double>();
        }

        for (auto& c : p->children) {
            prepareLayout(ReactFlexLayoutPrivate::get(ReactFlexLayout::get(c)));
        }
    }
    void setDirty(bool dirty) {
        // qDebug() << __PRETTY_FUNCTION__ << this << "old" << dirty << "new" << dirty;
        this->dirty = dirty;
        if (!dirty)
            return; // dont reset parent
        if (parentItem != nullptr) {
            ReactFlexLayout* pl = ReactFlexLayout::get(parentItem, false);
            if (pl != nullptr)
                pl->setDirty(dirty);
        }
    }
    void local_print_node(int tab) {
        for (int i = 0; i < tab; ++i)
            printf(" ");

        printf("%d:(%s(%p) - (%f,%f, %fx%f)\n",
               ReactAttachedProperties::get(item)->tag(),
               item->metaObject()->className(),
               (void*)item,
               item->x(),
               item->y(),
               item->width(),
               item->height());
        if (qmlImplicitHeight || qmlImplicitWidth) {
            for (int i = 0; i < tab; ++i)
                printf(" ");
            printf("item has using implicitWidth=%f, implictHeight=%f\n",
                   QQmlProperty(item, "implictWidth").read().value<double>(),
                   QQmlProperty(item, "implictHeight").read().value<double>());
        }

        for (int i = 0; i < tab + 2; ++i)
            printf(" ");
        print_css_node(cssNode, (css_print_options_t)(CSS_PRINT_LAYOUT | CSS_PRINT_STYLE));

        for (QQuickItem* c : children /*item->childItems()*/) {
            ReactFlexLayoutPrivate::get(ReactFlexLayout::get(c))->local_print_node(tab + 2);
        }
        fflush(stdout);
    }
    void layout() { // called only at top level
        // qDebug() << __PRETTY_FUNCTION__ << this;
        if (!dirty)
            return;

        prepareLayout(this);
        // qDebug() << __PRETTY_FUNCTION__ << "After prepareLayout";
        // local_print_node(0);

        layoutNode(cssNode, CSS_UNDEFINED, CSS_UNDEFINED, CSS_DIRECTION_INHERIT);
        // qDebug() << __PRETTY_FUNCTION__ << "After layoutNode";
        // local_print_node(0);

        applyLayout();
        // qDebug() << __PRETTY_FUNCTION__ << "After applyLayout";
        // local_print_node(0);
    }
    void setMeasureFunction(const measure_function& measureFunction) {
        this->measureFunction = measureFunction;
        cssNode->measure = measureFunction ? measure : nullptr;
    }
    static ReactFlexLayoutPrivate* get(ReactFlexLayout* rfl) {
        return rfl->d_func();
    }
    static css_node_t* getChild(void* context, int i) {
        const QList<QQuickItem*>& childItems = static_cast<ReactFlexLayoutPrivate*>(context)->children;

        return ReactFlexLayoutPrivate::get(ReactFlexLayout::get(childItems.at(i)))->cssNode;
    }
    static bool isDirty(void* context) {
        return static_cast<ReactFlexLayoutPrivate*>(context)->dirty;
    }
    static css_dim_t
    measure(void* context, float width, css_measure_mode_t widthMode, float height, css_measure_mode_t heightMode) {
        ReactFlexLayoutPrivate* p = static_cast<ReactFlexLayoutPrivate*>(context);
        if (!p->measureFunction) {
            qCritical() << "measure set without measure function?!";
            return {};
        }
        flex_dimensions df = p->measureFunction(width, FlexMeasureMode(widthMode), height, FlexMeasureMode(heightMode));
        return {float(df.first), float(df.second)};
    }
    bool qmlAnchors;
    bool qmlImplicitWidth;
    bool qmlImplicitHeight;
    bool dirty;
    float padding[CSS_PROP_COUNT];
    float margin[CSS_PROP_COUNT];
    QQuickItem* item;
    QQuickItem* parentItem;
    QList<QQuickItem*> children;
    css_node_t* cssNode;
    measure_function measureFunction;
    ReactFlexLayout* q_ptr;

private:
    void applyLayout() {
        // qDebug() << __PRETTY_FUNCTION__ << this << cssNode->layout.should_update;
        if (!cssNode->layout.should_update) {
            return;
        }
        cssNode->layout.should_update = false;
        dirty = false;

        if (!qmlAnchors) {
            item->setX(cssNode->layout.position[CSS_LEFT]);
            item->setY(cssNode->layout.position[CSS_TOP]);
            item->setWidth(cssNode->layout.dimensions[CSS_WIDTH]);
            item->setHeight(cssNode->layout.dimensions[CSS_HEIGHT]);
        }

        for (auto& c : children) {
            ReactFlexLayoutPrivate::get(ReactFlexLayout::get(c))->applyLayout();
        }

        cssNode->layout.position[CSS_LEFT] = 0;
        cssNode->layout.position[CSS_TOP] = 0;
        cssNode->layout.dimensions[CSS_WIDTH] = CSS_UNDEFINED;
        cssNode->layout.dimensions[CSS_HEIGHT] = CSS_UNDEFINED;
    }
};

QDebug operator<<(QDebug debug, const ReactFlexLayoutPrivate* p) {
    QDebugStateSaver s(debug);
    debug.nospace() << "ReactFlexLayoutPrivate(this=" << (void*)p
                    << ", tag=" << ReactAttachedProperties::get(p->item)->tag() << ", dirty=" << p->dirty
                    << ", item=" << p->item << ", parent=" << p->parentItem << ", layout={"
                    << p->cssNode->layout.position[CSS_LEFT] << " " << p->cssNode->layout.position[CSS_TOP] << " "
                    << p->cssNode->layout.dimensions[CSS_WIDTH] << " " << p->cssNode->layout.dimensions[CSS_HEIGHT]
                    << "})";
    return debug;
}

ReactFlexLayout::ReactFlexLayout(QObject* parent) : QObject(parent), d_ptr(new ReactFlexLayoutPrivate(this)) {
    Q_D(ReactFlexLayout);
    d->qmlAnchors = false;
    d->qmlImplicitWidth = false;
    d->qmlImplicitHeight = false;
    d->dirty = false;
    for (int i = 0; i < CSS_PROP_COUNT; ++i) {
        d->margin[i] = CSS_UNDEFINED;
        d->padding[i] = CSS_UNDEFINED;
    }
    d->item = qobject_cast<QQuickItem*>(parent);
    d->parentItem = nullptr;
    if (d->item == nullptr) {
        qCritical() << "Flex layout only applies to visual items";
    }
}

ReactFlexLayout::~ReactFlexLayout() {}

bool ReactFlexLayout::qmlAnchors() const {
    return d_func()->qmlAnchors;
}

void ReactFlexLayout::setQmlAnchors(bool qmlAnchors) {
    Q_D(ReactFlexLayout);
    if (d->qmlAnchors == qmlAnchors)
        return;
    d->qmlAnchors = qmlAnchors;
}

bool ReactFlexLayout::qmlImplicitWidth() const {
    return d_func()->qmlImplicitWidth;
}

void ReactFlexLayout::setQmlImplicitWidth(bool qmlImplicitWidth) {
    Q_D(ReactFlexLayout);
    if (d->qmlImplicitWidth == qmlImplicitWidth)
        return;
    d->qmlImplicitWidth = qmlImplicitWidth;
}

bool ReactFlexLayout::qmlImplicitHeight() const {
    return d_func()->qmlImplicitHeight;
}

void ReactFlexLayout::setQmlImplicitHeight(bool qmlImplicitHeight) {
    Q_D(ReactFlexLayout);
    if (d->qmlImplicitHeight == qmlImplicitHeight)
        return;
    d->qmlImplicitHeight = qmlImplicitHeight;
}

bool ReactFlexLayout::isDirty() {
    return d_func()->dirty;
}

void ReactFlexLayout::setDirty(bool dirty) {
    Q_D(ReactFlexLayout);
    if (d->dirty == dirty)
        return;
    d->setDirty(dirty);
}

double ReactFlexLayout::flex() const {
    return d_func()->cssNode->style.flex;
}

void ReactFlexLayout::setFlex(double flex) {
    Q_D(ReactFlexLayout);
    if (d->cssNode->style.flex == flex) // XXX:
        return;

    d->cssNode->style.flex = flex;
    setDirty(true);
    Q_EMIT flexChanged();
}

ReactFlexLayout::Direction ReactFlexLayout::flexDirection() const {
    return (ReactFlexLayout::Direction)d_func()->cssNode->style.flex_direction;
}

void ReactFlexLayout::setFlexDirection(Direction direction) {
    Q_D(ReactFlexLayout);
    if (d->cssNode->style.flex_direction == (css_flex_direction_t)direction)
        return;

    d->cssNode->style.flex_direction = (css_flex_direction_t)direction;
    setDirty(true);
    Q_EMIT flexDirectionChanged();
}

ReactFlexLayout::Justify ReactFlexLayout::justifyContent() const {
    return (ReactFlexLayout::Justify)d_func()->cssNode->style.justify_content;
}

void ReactFlexLayout::setJustifyContent(Justify justify) {
    Q_D(ReactFlexLayout);
    if (d->cssNode->style.justify_content == (css_justify_t)justify)
        return;

    d->cssNode->style.justify_content = (css_justify_t)justify;
    setDirty(true);
    Q_EMIT justifyContentChanged();
}

ReactFlexLayout::Alignment ReactFlexLayout::selfAlignment() const {
    return (ReactFlexLayout::Alignment)d_func()->cssNode->style.align_self;
}

void ReactFlexLayout::setSelfAlignment(Alignment alignment) {
    Q_D(ReactFlexLayout);
    if (d->cssNode->style.align_self == (css_align_t)alignment)
        return;

    d->cssNode->style.align_self = (css_align_t)alignment;
    setDirty(true);
    Q_EMIT selfAlignmentChanged();
}

ReactFlexLayout::Alignment ReactFlexLayout::alignItems() const {
    return (ReactFlexLayout::Alignment)d_func()->cssNode->style.align_items;
}

void ReactFlexLayout::setAlignItems(Alignment alignment) {
    Q_D(ReactFlexLayout);
    if (d->cssNode->style.align_items == (css_align_t)alignment)
        return;

    d->cssNode->style.align_items = (css_align_t)alignment;
    setDirty(true);
    Q_EMIT alignItemsChanged();
}

ReactFlexLayout::Position ReactFlexLayout::position() const {
    return (ReactFlexLayout::Position)d_func()->cssNode->style.position_type;
}

void ReactFlexLayout::setPosition(Position position) {
    Q_D(ReactFlexLayout);
    if (d->cssNode->style.position_type == (css_position_type_t)position)
        return;

    d->cssNode->style.position_type = (css_position_type_t)position;
    setDirty(true);
    Q_EMIT positionChanged();
}

ReactFlexLayout::Wrap ReactFlexLayout::wrap() const {
    return (ReactFlexLayout::Wrap)d_func()->cssNode->style.flex_wrap;
}

void ReactFlexLayout::setWrap(Wrap wrap) {
    Q_D(ReactFlexLayout);
    if (d->cssNode->style.flex_wrap == (css_wrap_type_t)wrap)
        return;

    d->cssNode->style.flex_wrap = (css_wrap_type_t)wrap;
    setDirty(true);
    Q_EMIT wrapChanged();
}

double ReactFlexLayout::top() const {
    return d_func()->cssNode->style.position[CSS_TOP];
}

void ReactFlexLayout::setTop(double top) {
    Q_D(ReactFlexLayout);
    if (d->cssNode->style.position[CSS_TOP] == top)
        return;

    d->cssNode->style.position[CSS_TOP] = top;
    setDirty(true);
    Q_EMIT topChanged();
}

double ReactFlexLayout::right() const {
    return d_func()->cssNode->style.position[CSS_RIGHT];
}

void ReactFlexLayout::setRight(double right) {
    Q_D(ReactFlexLayout);
    if (d->cssNode->style.position[CSS_RIGHT] == right)
        return;

    d->cssNode->style.position[CSS_RIGHT] = right;
    setDirty(true);
    Q_EMIT rightChanged();
}

double ReactFlexLayout::bottom() const {
    return d_func()->cssNode->style.position[CSS_BOTTOM];
}

void ReactFlexLayout::setBottom(double bottom) {
    Q_D(ReactFlexLayout);
    if (d->cssNode->style.position[CSS_BOTTOM] == bottom)
        return;

    d->cssNode->style.position[CSS_BOTTOM] = bottom;
    setDirty(true);
    Q_EMIT bottomChanged();
}

double ReactFlexLayout::left() const {
    return d_func()->cssNode->style.position[CSS_LEFT];
}

void ReactFlexLayout::setLeft(double left) {
    Q_D(ReactFlexLayout);
    if (d->cssNode->style.position[CSS_LEFT] == left)
        return;

    d->cssNode->style.position[CSS_LEFT] = left;
    setDirty(true);
    Q_EMIT leftChanged();
}

double ReactFlexLayout::width() const {
    return d_func()->cssNode->style.dimensions[CSS_WIDTH];
}

void ReactFlexLayout::setWidth(double width) {
    Q_D(ReactFlexLayout);
    if (d->cssNode->style.dimensions[CSS_WIDTH] == width)
        return;

    d->cssNode->style.dimensions[CSS_WIDTH] = width;
    setDirty(true);
    Q_EMIT widthChanged();
}

double ReactFlexLayout::height() const {
    return d_func()->cssNode->style.dimensions[CSS_HEIGHT];
}

void ReactFlexLayout::setHeight(double height) {
    Q_D(ReactFlexLayout);
    if (d->cssNode->style.dimensions[CSS_HEIGHT] == height)
        return;

    d->cssNode->style.dimensions[CSS_HEIGHT] = height;
    setDirty(true);
    Q_EMIT heightChanged();
}

double ReactFlexLayout::padding() const {
    return d_func()->cssNode->style.padding[CSS_TOP]; // XXX:
}

void ReactFlexLayout::setPadding(double padding) {
    Q_D(ReactFlexLayout);
    d->cssNode->style.padding[CSS_TOP] = d->cssNode->style.padding[CSS_LEFT] = d->cssNode->style.padding[CSS_BOTTOM] =
        d->cssNode->style.padding[CSS_RIGHT] = padding;
    setDirty(true);
}

double ReactFlexLayout::paddingVertical() const {
    return d_func()->padding[CSS_VERTICAL];
}

void ReactFlexLayout::setPaddingVertical(double padding) {
    Q_D(ReactFlexLayout);
    d->padding[CSS_VERTICAL] = padding;
    setDirty(true);
}

double ReactFlexLayout::paddingHorizontal() const {
    return d_func()->padding[CSS_HORIZONTAL];
}

void ReactFlexLayout::setPaddingHorizontal(double padding) {
    Q_D(ReactFlexLayout);
    d->padding[CSS_HORIZONTAL] = padding;
    setDirty(true);
}

double ReactFlexLayout::paddingTop() const {
    return d_func()->padding[CSS_TOP];
}

void ReactFlexLayout::setPaddingTop(double padding) {
    Q_D(ReactFlexLayout);
    d->padding[CSS_TOP] = padding;
    setDirty(true);
}

double ReactFlexLayout::paddingLeft() const {
    return d_func()->padding[CSS_LEFT];
}

void ReactFlexLayout::setPaddingLeft(double padding) {
    Q_D(ReactFlexLayout);
    d->padding[CSS_LEFT] = padding;
    setDirty(true);
}

double ReactFlexLayout::paddingBottom() const {
    return d_func()->padding[CSS_BOTTOM];
}

void ReactFlexLayout::setPaddingBottom(double padding) {
    Q_D(ReactFlexLayout);
    d->padding[CSS_BOTTOM] = padding;
    setDirty(true);
}

double ReactFlexLayout::paddingRight() const {
    return d_func()->padding[CSS_RIGHT];
}

void ReactFlexLayout::setPaddingRight(double padding) {
    Q_D(ReactFlexLayout);
    d->padding[CSS_RIGHT] = padding;
    setDirty(true);
}

double ReactFlexLayout::margin() const {
    return d_func()->cssNode->style.margin[CSS_TOP]; // XXX:
}
void ReactFlexLayout::setMargin(double margin) {
    Q_D(ReactFlexLayout);
    d->cssNode->style.margin[CSS_TOP] = d->cssNode->style.margin[CSS_LEFT] = d->cssNode->style.margin[CSS_BOTTOM] =
        d->cssNode->style.margin[CSS_RIGHT] = margin;
    setDirty(true);
}

double ReactFlexLayout::marginVertical() const {
    return d_func()->margin[CSS_VERTICAL];
}

void ReactFlexLayout::setMarginVertical(double margin) {
    Q_D(ReactFlexLayout);
    d->margin[CSS_VERTICAL] = margin;
    setDirty(true);
}

double ReactFlexLayout::marginHorizontal() const {
    return d_func()->margin[CSS_HORIZONTAL];
}

void ReactFlexLayout::setMarginHorizontal(double margin) {
    Q_D(ReactFlexLayout);
    d->margin[CSS_HORIZONTAL] = margin;
    setDirty(true);
}

double ReactFlexLayout::marginTop() const {
    return d_func()->margin[CSS_TOP];
}

void ReactFlexLayout::setMarginTop(double margin) {
    Q_D(ReactFlexLayout);
    d->margin[CSS_TOP] = margin;
    setDirty(true);
}

double ReactFlexLayout::marginLeft() const {
    return d_func()->margin[CSS_LEFT];
}

void ReactFlexLayout::setMarginLeft(double margin) {
    Q_D(ReactFlexLayout);
    d->margin[CSS_LEFT] = margin;
    setDirty(true);
}

double ReactFlexLayout::marginBottom() const {
    return d_func()->margin[CSS_BOTTOM];
}

void ReactFlexLayout::setMarginBottom(double margin) {
    Q_D(ReactFlexLayout);
    d->margin[CSS_BOTTOM] = margin;
    setDirty(true);
}

double ReactFlexLayout::marginRight() const {
    return d_func()->margin[CSS_RIGHT];
}

void ReactFlexLayout::setMarginRight(double margin) {
    Q_D(ReactFlexLayout);
    d->margin[CSS_RIGHT] = margin;
    setDirty(true);
}

measure_function ReactFlexLayout::measureFunction() const {
    return d_func()->measureFunction;
}

void ReactFlexLayout::setMeasureFunction(const measure_function& measureFunction) {
    d_func()->setMeasureFunction(measureFunction);
}

QQuickItem* ReactFlexLayout::parentItem() const {
    return d_func()->parentItem;
}

void ReactFlexLayout::setParentItem(QQuickItem* parentItem) {
    Q_D(ReactFlexLayout);
    d->parentItem = parentItem;
}

void ReactFlexLayout::insertChild(int position, QQuickItem* child) {
    Q_D(ReactFlexLayout);
    d->children.insert(position, child);
}

int ReactFlexLayout::getChildIndex(QQuickItem* child) {
    Q_D(ReactFlexLayout);
    int index = d->children.indexOf(child);
    Q_ASSERT(index >= 0);
    return index;
}

QList<QQuickItem*> ReactFlexLayout::removeChildren(const QList<int>& children) {
    Q_D(ReactFlexLayout);

    QList<QQuickItem*> removable;
    std::transform(children.begin(), children.end(), std::back_inserter(removable), [=](int index) {
        return d->children.at(index);
    });

    d->children.erase(std::remove_if(d->children.begin(),
                                     d->children.end(),
                                     [=](QQuickItem* item) { return removable.contains(item); }),
                      d->children.end());

    return removable;
}

void ReactFlexLayout::applyLayoutProperties(const QVariantMap& properties) {
    flexPropertyHandler()->applyProperties(this, properties);
}

void ReactFlexLayout::polish(QQuickItem* item) {
    ReactFlexLayout* rfl = ReactFlexLayout::get(item, false);
    if (rfl == nullptr) {
        qCritical() << "Attempting to start Flex layout on non-flex item!";
        return;
    }

    ReactFlexLayoutPrivate::get(rfl)->layout();
}

ReactFlexLayout* ReactFlexLayout::get(QQuickItem* item, bool create) {
    return qobject_cast<ReactFlexLayout*>(qmlAttachedPropertiesObject<ReactFlexLayout>(item, create));
}

ReactFlexLayout* ReactFlexLayout::qmlAttachedProperties(QObject* object) {
    return new ReactFlexLayout(object);
}
