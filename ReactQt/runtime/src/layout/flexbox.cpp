
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include "flexbox.h"
#include "attachedproperties.h"
#include "componentmanagers/viewmanager.h"

#include <QDebug>
#include <QMap>
#include <QQuickItem>

Q_LOGGING_CATEGORY(FLEXBOX, "Flexbox")

static QMap<QString, YGFlexDirection> flexDirectionByString{
    {"row", YGFlexDirectionRow},
    {"row-reverse", YGFlexDirectionRowReverse},
    {"column", YGFlexDirectionColumn},
    {"column-reverse", YGFlexDirectionColumnReverse},
    {"", YGFlexDirectionColumn},
};

static QMap<QString, YGJustify> justificationByString{
    {"flex-start", YGJustifyFlexStart},
    {"flex-end", YGJustifyFlexEnd},
    {"center", YGJustifyCenter},
    {"space-between", YGJustifySpaceBetween},
    {"space-around", YGJustifySpaceAround},
    {"", YGJustifyFlexStart},
};

static QMap<QString, YGAlign> alignByString{{"flex-start", YGAlignFlexStart},
                                            {"flex-end", YGAlignFlexEnd},
                                            {"center", YGAlignCenter},
                                            {"stretch", YGAlignStretch},
                                            {"baseline", YGAlignBaseline},
                                            {"space-between", YGAlignSpaceBetween},
                                            {"space-around", YGAlignSpaceAround},
                                            {"", YGAlignStretch}};

static QMap<QString, YGWrap> wrapByString{
    {"wrap", YGWrapWrap}, {"nowrap", YGWrapNoWrap}, {"", YGWrapNoWrap},
};

static QMap<QString, YGDisplay> displayByString{
    {"none", YGDisplayNone}, {"flex", YGDisplayFlex}, {"", YGDisplayFlex},
};

static QMap<QString, YGOverflow> overflowByString{
    {"visible", YGOverflowVisible}, {"hidden", YGOverflowHidden}, {"scroll", YGOverflowScroll}, {"", YGOverflowVisible},
};

static QMap<QString, YGPositionType> positionByString{
    {"relative", YGPositionTypeRelative}, {"absolute", YGPositionTypeAbsolute}, {"", YGPositionTypeRelative},
};

static QMap<QString, YGDirection> directionByString{
    {"inherit", YGDirectionInherit}, {"ltr", YGDirectionLTR}, {"rtl", YGDirectionRTL}, {"", YGDirectionInherit},
};

const char LAYOUT_UPDATED_SIGNAL_NAME[] = "layoutUpdated";

class FlexboxPrivate {
public:
    FlexboxPrivate() {
        m_node = YGNodeNew();
        YGNodeSetContext(m_node, this);
    }
    ~FlexboxPrivate() {
        YGNodeRef parent = YGNodeGetParent(m_node);
        if (parent) {
            YGNodeRemoveChild(parent, m_node);
        }
        YGNodeFree(m_node);
    }

    static void updatePropertiesForControlsTree(YGNodeRef node);
    static void updatePropertiesForControl(YGNodeRef node);

    void printNode();

    YGNodeRef m_node = nullptr;
    QString m_flexDirection;
    QString m_justifyContent;
    QQuickItem* m_control = nullptr;
    ViewManager* m_viewManager = nullptr;
    ygnode_measure_function m_measureFunction = nullptr;
    QString m_alignItems;
    QString m_alignContent;
    QString m_alignSelf;
    QString m_flexWrap;
    QString m_display;
    QString m_overflow;
    QString m_position;
    QString m_direction;
};

YGSize measure(YGNodeRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode) {
    FlexboxPrivate* d = static_cast<FlexboxPrivate*>(YGNodeGetContext(node));
    auto nodeMeasureFunction = d->m_measureFunction;
    return nodeMeasureFunction(node, width, widthMode, height, heightMode);
}

Flexbox::Flexbox(QObject* parent) : QObject(parent), d_ptr(new FlexboxPrivate()) {}

Flexbox::~Flexbox() {}

void Flexbox::recalculateLayout(float width, float height) {
    Q_D(Flexbox);

    YGNodeCalculateLayout(d->m_node, width, height, YGDirectionLTR);
    d->updatePropertiesForControlsTree(d->m_node);
}

void Flexbox::setMeasureFunction(ygnode_measure_function measureFunction) {
    Q_D(Flexbox);

    d->m_measureFunction = measureFunction;
    YGNodeSetMeasureFunc(d->m_node, measure);
}

void Flexbox::addChild(int index, Flexbox* child) {
    Q_ASSERT(child);

    qCDebug(FLEXBOX) << "::addChild new YGNode: " << child->d_ptr->m_node << " for flexbox: " << child
                     << " will be inserted at: " << index << " into parent: " << this;
    YGNodeInsertChild(d_ptr->m_node, child->d_ptr->m_node, index);
}

void Flexbox::removeChilds(const QList<int>& indicesToRemove) {
    Q_D(Flexbox);

    QList<YGNodeRef> toRemove;
    for (int index : indicesToRemove) {
        qCDebug(FLEXBOX) << "::removeChilds YGNode: " << YGNodeGetChild(d->m_node, index)
                         << " will be removed at index: " << index << " from: " << this;
        toRemove.push_back(YGNodeGetChild(d->m_node, index));
    }

    for (YGNodeRef nodeToRemove : toRemove) {
        YGNodeRemoveChild(d->m_node, nodeToRemove);
    }
}

Flexbox* Flexbox::findFlexbox(QQuickItem* control) {
    Q_ASSERT(control);
    Flexbox* flexbox = control->property("flexbox").value<Flexbox*>();
    return flexbox;
}

void FlexboxPrivate::updatePropertiesForControlsTree(YGNodeRef node) {
    static int level = 1;

    updatePropertiesForControl(node);

    int childCount = YGNodeGetChildCount(node);
    for (int i = 0; i < childCount; ++i) {
        updatePropertiesForControlsTree(YGNodeGetChild(node, i));
    }
}

void FlexboxPrivate::updatePropertiesForControl(YGNodeRef node) {
    auto qmlControl = static_cast<FlexboxPrivate*>(YGNodeGetContext(node))->m_control;
    auto viewManager = static_cast<FlexboxPrivate*>(YGNodeGetContext(node))->m_viewManager;

    bool layoutUpdated = YGNodeLayoutGetLeft(node) != qmlControl->x() || YGNodeLayoutGetTop(node) != qmlControl->y() ||
                         YGNodeLayoutGetWidth(node) != qmlControl->width() ||
                         YGNodeLayoutGetHeight(node) != qmlControl->height();

    if (layoutUpdated) {
        qmlControl->setX(YGNodeLayoutGetLeft(node));
        qmlControl->setY(YGNodeLayoutGetTop(node));
        qmlControl->setWidth(YGNodeLayoutGetWidth(node));
        qmlControl->setHeight(YGNodeLayoutGetHeight(node));
    }

    if (layoutUpdated && viewManager) {
        viewManager->sendOnLayoutToJs(qmlControl,
                                      YGNodeLayoutGetLeft(node),
                                      YGNodeLayoutGetTop(node),
                                      YGNodeLayoutGetWidth(node),
                                      YGNodeLayoutGetHeight(node));
    }
}

void Flexbox::setControl(QQuickItem* value) {
    if (value != d_ptr->m_control) {
        d_ptr->m_control = value;
        controlChanged();
    }
}

QObject* Flexbox::viewManager() {
    return d_ptr->m_viewManager;
}

void Flexbox::setViewManager(QObject* value) {
    ViewManager* viewManager = qobject_cast<ViewManager*>(value);
    if (viewManager != d_ptr->m_viewManager) {
        d_ptr->m_viewManager = viewManager;
        viewManagerChanged();
    }
}

QQuickItem* Flexbox::control() {
    return d_ptr->m_control;
}

float Flexbox::width() {
    return YGNodeStyleGetWidth(d_ptr->m_node).value;
}

void Flexbox::setWidth(QVariant value) {
    float percents;
    if (parsePercents(value, percents)) {
        YGNodeStyleSetWidthPercent(d_ptr->m_node, percents);
        widthChanged();
    } else if (value.toFloat() != width()) {
        YGNodeStyleSetWidth(d_ptr->m_node, value.toFloat());
        widthChanged();
    }
}

float Flexbox::height() {
    return YGNodeStyleGetHeight(d_ptr->m_node).value;
}

void Flexbox::setHeight(QVariant value) {
    float percents;
    if (parsePercents(value, percents)) {
        YGNodeStyleSetHeightPercent(d_ptr->m_node, percents);
        heightChanged();
    } else if (value.toFloat() != height()) {
        YGNodeStyleSetHeight(d_ptr->m_node, value.toFloat());
        heightChanged();
    }
}

QString Flexbox::flexDirection() {
    return d_ptr->m_flexDirection;
}

void Flexbox::setFlexDirection(const QString& value) {
    if (value != d_ptr->m_flexDirection) {
        d_ptr->m_flexDirection = value;
        YGNodeStyleSetFlexDirection(d_ptr->m_node, flexDirectionByString[value]);
        flexDirectionChanged();
    }
}

QString Flexbox::justifyContent() {
    return d_ptr->m_justifyContent;
}

void Flexbox::setJustifyContent(const QString& value) {
    if (value != d_ptr->m_justifyContent) {
        d_ptr->m_justifyContent = value;
        YGNodeStyleSetJustifyContent(d_ptr->m_node, justificationByString[value]);
        justifyContentChanged();
    }
}

float Flexbox::margin() {
    return YGNodeStyleGetMargin(d_ptr->m_node, YGEdgeAll).value;
}

void Flexbox::setMargin(QVariant value) {
    float percents;
    if (parsePercents(value, percents)) {
        YGNodeStyleSetMarginPercent(d_ptr->m_node, YGEdgeAll, percents);
        marginChanged();
    } else if (value.toFloat() != margin()) {
        YGNodeStyleSetMargin(d_ptr->m_node, YGEdgeAll, value.toFloat());
        marginChanged();
    }
}

float Flexbox::marginTop() {
    return YGNodeStyleGetMargin(d_ptr->m_node, YGEdgeTop).value;
}

void Flexbox::setMarginTop(QVariant value) {
    float percents;
    if (parsePercents(value, percents)) {
        YGNodeStyleSetMarginPercent(d_ptr->m_node, YGEdgeTop, percents);
        marginTopChanged();
    } else if (value.toFloat() != marginTop()) {
        YGNodeStyleSetMargin(d_ptr->m_node, YGEdgeTop, value.toFloat());
        marginTopChanged();
    }
}

float Flexbox::marginBottom() {
    return YGNodeStyleGetMargin(d_ptr->m_node, YGEdgeBottom).value;
}

void Flexbox::setMarginBottom(QVariant value) {
    float percents;
    if (parsePercents(value, percents)) {
        YGNodeStyleSetMarginPercent(d_ptr->m_node, YGEdgeBottom, percents);
        marginBottomChanged();
    } else if (value.toFloat() != marginBottom()) {
        YGNodeStyleSetMargin(d_ptr->m_node, YGEdgeBottom, value.toFloat());
        marginBottomChanged();
    }
}

float Flexbox::marginLeft() {
    return YGNodeStyleGetMargin(d_ptr->m_node, YGEdgeLeft).value;
}

void Flexbox::setMarginLeft(QVariant value) {
    float percents;
    if (parsePercents(value, percents)) {
        YGNodeStyleSetMarginPercent(d_ptr->m_node, YGEdgeLeft, percents);
        marginLeftChanged();
    } else if (value.toFloat() != marginLeft()) {
        YGNodeStyleSetMargin(d_ptr->m_node, YGEdgeLeft, value.toFloat());
        marginLeftChanged();
    }
}

float Flexbox::marginRight() {
    return YGNodeStyleGetMargin(d_ptr->m_node, YGEdgeRight).value;
}

void Flexbox::setMarginRight(QVariant value) {
    float percents;
    if (parsePercents(value, percents)) {
        YGNodeStyleSetMarginPercent(d_ptr->m_node, YGEdgeRight, percents);
        marginRightChanged();
    } else if (value.toFloat() != marginRight()) {
        YGNodeStyleSetMargin(d_ptr->m_node, YGEdgeRight, value.toFloat());
        marginRightChanged();
    }
}

float Flexbox::marginHorizontal() {
    return YGNodeStyleGetMargin(d_ptr->m_node, YGEdgeHorizontal).value;
}

void Flexbox::setMarginHorizontal(QVariant value) {
    float percents;
    if (parsePercents(value, percents)) {
        YGNodeStyleSetMarginPercent(d_ptr->m_node, YGEdgeHorizontal, percents);
        marginHorizontalChanged();
    } else if (value.toFloat() != marginHorizontal()) {
        YGNodeStyleSetMargin(d_ptr->m_node, YGEdgeHorizontal, value.toFloat());
        marginHorizontalChanged();
    }
}

float Flexbox::marginVertical() {
    return YGNodeStyleGetMargin(d_ptr->m_node, YGEdgeVertical).value;
}

void Flexbox::setMarginVertical(QVariant value) {
    float percents;
    if (parsePercents(value, percents)) {
        YGNodeStyleSetMarginPercent(d_ptr->m_node, YGEdgeVertical, percents);
        marginVerticalChanged();
    } else if (value.toFloat() != marginVertical()) {
        YGNodeStyleSetMargin(d_ptr->m_node, YGEdgeVertical, value.toFloat());
        marginVerticalChanged();
    }
}

float Flexbox::borderWidth() {
    return YGNodeStyleGetBorder(d_ptr->m_node, YGEdgeAll);
}

void Flexbox::setBorderWidth(float value) {
    if (value != borderWidth()) {
        YGNodeStyleSetBorder(d_ptr->m_node, YGEdgeAll, value);
        borderWidthChanged();
    }
}

float Flexbox::borderTopWidth() {
    return YGNodeStyleGetBorder(d_ptr->m_node, YGEdgeTop);
}

void Flexbox::setBorderTopWidth(float value) {
    if (value != borderTopWidth()) {
        YGNodeStyleSetBorder(d_ptr->m_node, YGEdgeTop, value);
        borderTopWidthChanged();
    }
}

float Flexbox::borderBottomWidth() {
    return YGNodeStyleGetBorder(d_ptr->m_node, YGEdgeBottom);
}

void Flexbox::setBorderBottomWidth(float value) {
    if (value != borderBottomWidth()) {
        YGNodeStyleSetBorder(d_ptr->m_node, YGEdgeBottom, value);
        borderBottomWidthChanged();
    }
}

float Flexbox::borderLeftWidth() {
    return YGNodeStyleGetBorder(d_ptr->m_node, YGEdgeLeft);
}

void Flexbox::setBorderLeftWidth(float value) {
    if (value != borderLeftWidth()) {
        YGNodeStyleSetBorder(d_ptr->m_node, YGEdgeLeft, value);
        borderLeftWidthChanged();
    }
}

float Flexbox::borderRightWidth() {
    return YGNodeStyleGetBorder(d_ptr->m_node, YGEdgeRight);
}

void Flexbox::setBorderRightWidth(float value) {
    if (value != borderRightWidth()) {
        YGNodeStyleSetBorder(d_ptr->m_node, YGEdgeRight, value);
        borderRightWidthChanged();
    }
}

QString Flexbox::alignItems() {
    return d_ptr->m_alignItems;
}

void Flexbox::setAlignItems(const QString& value) {
    if (value != d_ptr->m_alignItems) {
        d_ptr->m_alignItems = value;
        YGNodeStyleSetAlignItems(d_ptr->m_node, alignByString[value]);
        alignItemsChanged();
    }
}

QString Flexbox::alignContent() {
    return d_ptr->m_alignContent;
}

void Flexbox::setAlignContent(const QString& value) {
    if (value != d_ptr->m_alignContent) {
        d_ptr->m_alignContent = value;
        YGNodeStyleSetAlignContent(d_ptr->m_node, alignByString[value]);
        alignContentChanged();
    }
}

QString Flexbox::alignSelf() {
    return d_ptr->m_alignSelf;
}

void Flexbox::setAlignSelf(const QString& value) {
    if (value != d_ptr->m_alignSelf) {
        d_ptr->m_alignSelf = value;
        YGNodeStyleSetAlignSelf(d_ptr->m_node, alignByString[value]);
        alignSelfChanged();
    }
}

float Flexbox::aspectRatio() {
    return YGNodeStyleGetAspectRatio(d_ptr->m_node);
}

void Flexbox::setAspectRatio(float value) {
    if (value != aspectRatio()) {
        YGNodeStyleSetAspectRatio(d_ptr->m_node, value);
        aspectRatioChanged();
    }
}

float Flexbox::top() {
    return YGNodeLayoutGetTop(d_ptr->m_node);
}

void Flexbox::setTop(QVariant value) {
    float percents;
    if (parsePercents(value, percents)) {
        YGNodeStyleSetPositionPercent(d_ptr->m_node, YGEdgeTop, percents);
    } else {
        YGNodeStyleSetPosition(d_ptr->m_node, YGEdgeTop, value.toFloat());
    }
    topChanged();
}

float Flexbox::bottom() {
    return YGNodeLayoutGetBottom(d_ptr->m_node);
}

void Flexbox::setBottom(QVariant value) {

    float percents;
    if (parsePercents(value, percents)) {
        YGNodeStyleSetPositionPercent(d_ptr->m_node, YGEdgeBottom, percents);
    } else {
        YGNodeStyleSetPosition(d_ptr->m_node, YGEdgeBottom, value.toFloat());
    }
    bottomChanged();
}

float Flexbox::left() {
    return YGNodeLayoutGetLeft(d_ptr->m_node);
}

void Flexbox::setLeft(QVariant value) {
    float percents;
    if (parsePercents(value, percents)) {
        YGNodeStyleSetPositionPercent(d_ptr->m_node, YGEdgeLeft, percents);
    } else {
        YGNodeStyleSetPosition(d_ptr->m_node, YGEdgeLeft, value.toFloat());
    }
    leftChanged();
}

float Flexbox::right() {
    return YGNodeLayoutGetRight(d_ptr->m_node);
}
void Flexbox::setRight(QVariant value) {
    float percents;
    if (parsePercents(value, percents)) {
        YGNodeStyleSetPositionPercent(d_ptr->m_node, YGEdgeRight, percents);
    } else {
        YGNodeStyleSetPosition(d_ptr->m_node, YGEdgeRight, value.toFloat());
    }
    rightChanged();
}

float Flexbox::minWidth() {
    return YGNodeStyleGetMinWidth(d_ptr->m_node).value;
}

void Flexbox::setMinWidth(QVariant value) {
    float percents;
    if (parsePercents(value, percents)) {
        YGNodeStyleSetMinWidthPercent(d_ptr->m_node, percents);
        minWidthChanged();
    } else if (value.toFloat() != minWidth()) {
        YGNodeStyleSetMinWidth(d_ptr->m_node, value.toFloat());
        minWidthChanged();
    }
}

float Flexbox::minHeight() {
    return YGNodeStyleGetMinHeight(d_ptr->m_node).value;
}

void Flexbox::setMinHeight(QVariant value) {
    float percents;
    if (parsePercents(value, percents)) {
        YGNodeStyleSetMinHeightPercent(d_ptr->m_node, percents);
        minHeightChanged();
    } else if (value.toFloat() != minHeight()) {
        YGNodeStyleSetMinHeight(d_ptr->m_node, value.toFloat());
        minHeightChanged();
    }
}

float Flexbox::maxWidth() {
    return YGNodeStyleGetMaxWidth(d_ptr->m_node).value;
}

void Flexbox::setMaxWidth(QVariant value) {
    float percents;
    if (parsePercents(value, percents)) {
        YGNodeStyleSetMaxWidthPercent(d_ptr->m_node, percents);
        maxWidthChanged();
    } else if (value.toFloat() != maxWidth()) {
        YGNodeStyleSetMaxWidth(d_ptr->m_node, value.toFloat());
        maxWidthChanged();
    }
}

float Flexbox::maxHeight() {
    return YGNodeStyleGetMaxHeight(d_ptr->m_node).value;
}

void Flexbox::setMaxHeight(QVariant value) {
    float percents;
    if (parsePercents(value, percents)) {
        YGNodeStyleSetMaxHeightPercent(d_ptr->m_node, percents);
        maxHeightChanged();
    } else if (value.toFloat() != maxHeight()) {
        YGNodeStyleSetMaxHeight(d_ptr->m_node, value.toFloat());
        maxHeightChanged();
    }
}

float Flexbox::flex() {
    return YGNodeStyleGetFlex(d_ptr->m_node);
}

void Flexbox::setFlex(float value) {
    if (value != flex()) {
        YGNodeStyleSetFlex(d_ptr->m_node, value);
        flexChanged();
    }
}

float Flexbox::flexBasis() {
    return YGNodeStyleGetFlexBasis(d_ptr->m_node).value;
}

void Flexbox::setFlexBasis(QVariant value) {
    float percents;
    if (parsePercents(value, percents)) {
        YGNodeStyleSetFlexBasisPercent(d_ptr->m_node, percents);
        flexBasisChanged();
    } else if (value.toFloat() != flexBasis()) {
        YGNodeStyleSetFlexBasis(d_ptr->m_node, value.toFloat());
        flexBasisChanged();
    }
}

float Flexbox::flexGrow() {
    return YGNodeStyleGetFlexGrow(d_ptr->m_node);
}

void Flexbox::setFlexGrow(float value) {
    if (value != flexGrow()) {
        YGNodeStyleSetFlexGrow(d_ptr->m_node, value);
        flexGrowChanged();
    }
}

float Flexbox::flexShrink() {
    return YGNodeStyleGetFlexShrink(d_ptr->m_node);
}

void Flexbox::setFlexShrink(float value) {
    if (value != flexShrink()) {
        YGNodeStyleSetFlexShrink(d_ptr->m_node, value);
        flexShrinkChanged();
    }
}

QString Flexbox::flexWrap() {
    return d_ptr->m_flexWrap;
}

void Flexbox::setFlexWrap(const QString& value) {
    if (value != d_ptr->m_flexWrap) {
        d_ptr->m_flexWrap = value;
        YGNodeStyleSetFlexWrap(d_ptr->m_node, wrapByString[value]);
        flexWrapChanged();
    }
}

float Flexbox::padding() {
    return YGNodeStyleGetPadding(d_ptr->m_node, YGEdgeAll).value;
}

void Flexbox::setPadding(QVariant value) {
    float percents;
    if (parsePercents(value, percents)) {
        YGNodeStyleSetPadding(d_ptr->m_node, YGEdgeAll, percents);
        paddingChanged();
    } else if (value.toFloat() != padding()) {
        YGNodeStyleSetPadding(d_ptr->m_node, YGEdgeAll, value.toFloat());
        paddingChanged();
    }
}

float Flexbox::paddingTop() {
    return YGNodeStyleGetPadding(d_ptr->m_node, YGEdgeTop).value;
}

void Flexbox::setPaddingTop(QVariant value) {
    float percents;
    if (parsePercents(value, percents)) {
        YGNodeStyleSetPadding(d_ptr->m_node, YGEdgeTop, percents);
        paddingTopChanged();
    } else if (value.toFloat() != paddingTop()) {
        YGNodeStyleSetPadding(d_ptr->m_node, YGEdgeTop, value.toFloat());
        paddingTopChanged();
    }
}

float Flexbox::paddingBottom() {
    return YGNodeStyleGetPadding(d_ptr->m_node, YGEdgeBottom).value;
}

void Flexbox::setPaddingBottom(QVariant value) {
    float percents;
    if (parsePercents(value, percents)) {
        YGNodeStyleSetPadding(d_ptr->m_node, YGEdgeBottom, percents);
        paddingBottomChanged();
    } else if (value.toFloat() != paddingBottom()) {
        YGNodeStyleSetPadding(d_ptr->m_node, YGEdgeBottom, value.toFloat());
        paddingBottomChanged();
    }
}

float Flexbox::paddingLeft() {
    return YGNodeStyleGetPadding(d_ptr->m_node, YGEdgeLeft).value;
}

void Flexbox::setPaddingLeft(QVariant value) {
    float percents;
    if (parsePercents(value, percents)) {
        YGNodeStyleSetPadding(d_ptr->m_node, YGEdgeLeft, percents);
        paddingLeftChanged();
    } else if (value.toFloat() != paddingLeft()) {
        YGNodeStyleSetPadding(d_ptr->m_node, YGEdgeLeft, value.toFloat());
        paddingLeftChanged();
    }
}

float Flexbox::paddingRight() {
    return YGNodeStyleGetPadding(d_ptr->m_node, YGEdgeRight).value;
}

void Flexbox::setPaddingRight(QVariant value) {
    float percents;
    if (parsePercents(value, percents)) {
        YGNodeStyleSetPadding(d_ptr->m_node, YGEdgeRight, percents);
        paddingRightChanged();
    } else if (value.toFloat() != paddingRight()) {
        YGNodeStyleSetPadding(d_ptr->m_node, YGEdgeRight, value.toFloat());
        paddingRightChanged();
    }
}

float Flexbox::paddingHorizontal() {
    return YGNodeStyleGetPadding(d_ptr->m_node, YGEdgeHorizontal).value;
}

void Flexbox::setPaddingHorizontal(QVariant value) {
    float percents;
    if (parsePercents(value, percents)) {
        YGNodeStyleSetPadding(d_ptr->m_node, YGEdgeHorizontal, percents);
        paddingHorizontalChanged();
    } else if (value.toFloat() != paddingHorizontal()) {
        YGNodeStyleSetPadding(d_ptr->m_node, YGEdgeHorizontal, value.toFloat());
        paddingHorizontalChanged();
    }
}

float Flexbox::paddingVertical() {
    return YGNodeStyleGetPadding(d_ptr->m_node, YGEdgeVertical).value;
}

void Flexbox::setPaddingVertical(QVariant value) {
    float percents;
    if (parsePercents(value, percents)) {
        YGNodeStyleSetPadding(d_ptr->m_node, YGEdgeVertical, percents);
        paddingVerticalChanged();
    } else if (value.toFloat() != paddingVertical()) {
        YGNodeStyleSetPadding(d_ptr->m_node, YGEdgeVertical, value.toFloat());
        paddingVerticalChanged();
    }
}

QString Flexbox::display() {
    return d_ptr->m_display;
}

void Flexbox::setDisplay(const QString& value) {
    if (value != d_ptr->m_display) {
        d_ptr->m_display = value;
        YGNodeStyleSetDisplay(d_ptr->m_node, displayByString[value]);
        displayChanged();
    }
}

QString Flexbox::overflow() {
    return d_ptr->m_overflow;
}

void Flexbox::setOverflow(const QString& value) {
    if (value != d_ptr->m_overflow) {
        d_ptr->m_overflow = value;
        YGNodeStyleSetOverflow(d_ptr->m_node, overflowByString[value]);
        overflowChanged();
    }
}

QString Flexbox::position() {
    return d_ptr->m_position;
}

void Flexbox::setPosition(const QString& value) {
    if (value != d_ptr->m_position) {
        d_ptr->m_position = value;
        YGNodeStyleSetPositionType(d_ptr->m_node, positionByString[value]);
        positionChanged();
    }
}

QString Flexbox::direction() {
    return d_ptr->m_direction;
}

void Flexbox::setDirection(const QString& value) {
    if (value != d_ptr->m_direction) {
        d_ptr->m_direction = value;
        YGNodeStyleSetDirection(d_ptr->m_node, directionByString[value]);
        directionChanged();
    }
}

bool Flexbox::isDirty() {
    return YGNodeIsDirty(d_ptr->m_node);
}

bool Flexbox::parsePercents(QVariant& value, float& result) {
    if (value.type() == QMetaType::QString) {
        QString valueString = value.toString();
        if (valueString.back() == '%') {
            valueString.chop(1);
            bool success = false;
            result = valueString.toFloat(&success);
            if (success) {
                return true;
            }
        }
        qCDebug(FLEXBOX) << "Unable to parse value " << valueString;
    }
    return false;
}

void Flexbox::markDirty() {
    if (d_ptr->m_node) {
        YGNodeMarkDirty(d_ptr->m_node);
    }
}

bool Flexbox::isUndefined(float value) {
    return YGFloatIsUndefined(value);
}

void Flexbox::printFlexboxHierarchy() {
    d_ptr->printNode();
}

void printYGNode(YGNodeRef node, const QString& nodeName) {
    qDebug() << nodeName << ": width: " << YGNodeLayoutGetWidth(node) << " height: " << YGNodeLayoutGetHeight(node)
             << " x: " << YGNodeLayoutGetLeft(node) << " y: " << YGNodeLayoutGetTop(node);
}

void FlexboxPrivate::printNode() {
    static int level = 1;

    QString separator(level, '-');
    qDebug() << separator << " - " << m_control;
    printYGNode(m_node, "");

    ++level;
    for (int i = 0; i < YGNodeGetChildCount(m_node); ++i) {
        YGNodeRef childNode = YGNodeGetChild(m_node, i);
        FlexboxPrivate* fp = static_cast<FlexboxPrivate*>(YGNodeGetContext(childNode));
        fp->printNode();
    }
    --level;
}

#include "flexbox.moc"
