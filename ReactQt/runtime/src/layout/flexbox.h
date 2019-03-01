
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#ifndef FLEXBOX_H
#define FLEXBOX_H

#include <QLoggingCategory>
#include <QObject>
#include <functional>

Q_DECLARE_LOGGING_CATEGORY(FLEXBOX)

#include "../../../ReactCommon/yoga/yoga/Yoga.h"

typedef std::function<YGSize(
    YGNodeRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode)>
    ygnode_measure_function;

class QQuickItem;
class FlexboxPrivate;
class Flexbox : public QObject {

    Q_OBJECT
    Q_DECLARE_PRIVATE(Flexbox)

    Q_PROPERTY(QQuickItem* control READ control WRITE setControl NOTIFY controlChanged)
    Q_PROPERTY(QObject* viewManager READ viewManager WRITE setViewManager NOTIFY viewManagerChanged)
    Q_PROPERTY(QVariant p_width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(QVariant p_height READ height WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(QString p_flexDirection READ flexDirection WRITE setFlexDirection NOTIFY flexDirectionChanged)
    Q_PROPERTY(QString p_justifyContent READ justifyContent WRITE setJustifyContent NOTIFY justifyContentChanged)
    Q_PROPERTY(QVariant p_margin READ margin WRITE setMargin NOTIFY marginChanged)
    Q_PROPERTY(QVariant p_marginTop READ marginTop WRITE setMarginTop NOTIFY marginTopChanged)
    Q_PROPERTY(QVariant p_marginBottom READ marginBottom WRITE setMarginBottom NOTIFY marginBottomChanged)
    Q_PROPERTY(QVariant p_marginLeft READ marginLeft WRITE setMarginLeft NOTIFY marginLeftChanged)
    Q_PROPERTY(QVariant p_marginRight READ marginRight WRITE setMarginRight NOTIFY marginRightChanged)
    Q_PROPERTY(
        QVariant p_marginHorizontal READ marginHorizontal WRITE setMarginHorizontal NOTIFY marginHorizontalChanged)
    Q_PROPERTY(QVariant p_marginVertical READ marginVertical WRITE setMarginVertical NOTIFY marginVerticalChanged)
    Q_PROPERTY(float p_borderWidth READ borderWidth WRITE setBorderWidth NOTIFY borderWidthChanged)
    Q_PROPERTY(float p_borderTopWidth READ borderTopWidth WRITE setBorderTopWidth NOTIFY borderTopWidthChanged)
    Q_PROPERTY(
        float p_borderBottomWidth READ borderBottomWidth WRITE setBorderBottomWidth NOTIFY borderBottomWidthChanged)
    Q_PROPERTY(float p_borderLeftWidth READ borderLeftWidth WRITE setBorderLeftWidth NOTIFY borderLeftWidthChanged)
    Q_PROPERTY(float p_borderRightWidth READ borderRightWidth WRITE setBorderRightWidth NOTIFY borderRightWidthChanged)
    Q_PROPERTY(QString p_alignItems READ alignItems WRITE setAlignItems NOTIFY alignItemsChanged)
    Q_PROPERTY(QString p_alignContent READ alignContent WRITE setAlignContent NOTIFY alignContentChanged)
    Q_PROPERTY(QString p_alignSelf READ alignSelf WRITE setAlignSelf NOTIFY alignSelfChanged)
    Q_PROPERTY(float p_aspectRatio READ aspectRatio WRITE setAspectRatio NOTIFY aspectRatioChanged)
    Q_PROPERTY(QVariant p_bottom READ bottom WRITE setBottom NOTIFY bottomChanged)
    Q_PROPERTY(QVariant p_left READ left WRITE setLeft NOTIFY leftChanged)
    Q_PROPERTY(QVariant p_right READ right WRITE setRight NOTIFY rightChanged)
    Q_PROPERTY(QVariant p_top READ top WRITE setTop NOTIFY topChanged)
    Q_PROPERTY(QVariant p_maxWidth READ maxWidth WRITE setMaxWidth NOTIFY maxWidthChanged)
    Q_PROPERTY(QVariant p_maxHeight READ maxHeight WRITE setMaxHeight NOTIFY maxHeightChanged)
    Q_PROPERTY(QVariant p_minWidth READ minWidth WRITE setMinWidth NOTIFY minWidthChanged)
    Q_PROPERTY(QVariant p_minHeight READ minHeight WRITE setMinHeight NOTIFY minHeightChanged)
    Q_PROPERTY(float p_flex READ flex WRITE setFlex NOTIFY flexChanged)
    Q_PROPERTY(QVariant p_flexBasis READ flexBasis WRITE setFlexBasis NOTIFY flexBasisChanged)
    Q_PROPERTY(float p_flexGrow READ flexGrow WRITE setFlexGrow NOTIFY flexGrowChanged)
    Q_PROPERTY(float p_flexShrink READ flexShrink WRITE setFlexShrink NOTIFY flexShrinkChanged)
    Q_PROPERTY(QString p_flexWrap READ flexWrap WRITE setFlexWrap NOTIFY flexWrapChanged)
    Q_PROPERTY(float p_padding READ padding WRITE setPadding NOTIFY paddingChanged)
    Q_PROPERTY(float p_paddingTop READ paddingTop WRITE setPaddingTop NOTIFY paddingTopChanged)
    Q_PROPERTY(float p_paddingBottom READ paddingBottom WRITE setPaddingBottom NOTIFY paddingBottomChanged)
    Q_PROPERTY(float p_paddingLeft READ paddingLeft WRITE setPaddingLeft NOTIFY paddingLeftChanged)
    Q_PROPERTY(float p_paddingRight READ paddingRight WRITE setPaddingRight NOTIFY paddingRightChanged)
    Q_PROPERTY(
        float p_paddingHorizontal READ paddingHorizontal WRITE setPaddingHorizontal NOTIFY paddingHorizontalChanged)
    Q_PROPERTY(float p_paddingVertical READ paddingVertical WRITE setPaddingVertical NOTIFY paddingVerticalChanged)
    Q_PROPERTY(QString p_display READ display WRITE setDisplay NOTIFY displayChanged)
    Q_PROPERTY(QString p_overflow READ overflow WRITE setOverflow NOTIFY overflowChanged)
    Q_PROPERTY(QString p_position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(QString p_direction READ direction WRITE setDirection NOTIFY directionChanged)
    Q_PROPERTY(bool isDirty READ isDirty NOTIFY isDirtyChanged)

signals:
    void widthChanged();
    void heightChanged();
    void flexDirectionChanged();
    void justifyContentChanged();
    void controlChanged();
    void viewManagerChanged();
    void marginChanged();
    void marginTopChanged();
    void marginBottomChanged();
    void marginLeftChanged();
    void marginRightChanged();
    void marginHorizontalChanged();
    void marginVerticalChanged();
    void borderWidthChanged();
    void borderTopWidthChanged();
    void borderBottomWidthChanged();
    void borderLeftWidthChanged();
    void borderRightWidthChanged();
    void alignItemsChanged();
    void alignContentChanged();
    void alignSelfChanged();
    void aspectRatioChanged();
    void topChanged();
    void bottomChanged();
    void leftChanged();
    void rightChanged();
    void maxWidthChanged();
    void maxHeightChanged();
    void minWidthChanged();
    void minHeightChanged();
    void flexChanged();
    void flexGrowChanged();
    void flexBasisChanged();
    void flexShrinkChanged();
    void flexWrapChanged();
    void paddingChanged();
    void paddingTopChanged();
    void paddingBottomChanged();
    void paddingLeftChanged();
    void paddingRightChanged();
    void paddingHorizontalChanged();
    void paddingVerticalChanged();
    void displayChanged();
    void overflowChanged();
    void positionChanged();
    void directionChanged();
    void isDirtyChanged();
    //    void recalculated();

public:
    Flexbox(QObject* parent = 0);
    ~Flexbox();

    void recalculateLayout(float width, float height);
    void setMeasureFunction(ygnode_measure_function measureFunction);
    void addChild(int index, Flexbox* child);
    void removeChilds(const QList<int>& indicesToRemove);
    void printFlexboxHierarchy();

    static Flexbox* findFlexbox(QQuickItem* control);

    QQuickItem* control();
    void setControl(QQuickItem* value);
    QObject* viewManager();
    void setViewManager(QObject* value);
    float width();
    void setWidth(QVariant value);
    float height();
    void setHeight(QVariant value);
    QString flexDirection();
    void setFlexDirection(const QString& value);
    QString justifyContent();
    void setJustifyContent(const QString& value);
    float margin();
    void setMargin(QVariant value);
    float marginTop();
    void setMarginTop(QVariant value);
    float marginBottom();
    void setMarginBottom(QVariant value);
    float marginLeft();
    void setMarginLeft(QVariant value);
    float marginRight();
    void setMarginRight(QVariant value);
    float marginHorizontal();
    void setMarginHorizontal(QVariant value);
    float marginVertical();
    void setMarginVertical(QVariant value);
    float borderWidth();
    void setBorderWidth(float value);
    float borderTopWidth();
    void setBorderTopWidth(float value);
    float borderBottomWidth();
    void setBorderBottomWidth(float value);
    float borderLeftWidth();
    void setBorderLeftWidth(float value);
    float borderRightWidth();
    void setBorderRightWidth(float value);
    QString alignItems();
    void setAlignItems(const QString& value);
    QString alignContent();
    void setAlignContent(const QString& value);
    QString alignSelf();
    void setAlignSelf(const QString& value);
    float aspectRatio();
    void setAspectRatio(float value);
    float top();
    void setTop(QVariant value);
    float bottom();
    void setBottom(QVariant value);
    float left();
    void setLeft(QVariant value);
    float right();
    void setRight(QVariant value);
    float minWidth();
    void setMinWidth(QVariant value);
    float minHeight();
    void setMinHeight(QVariant value);
    float maxWidth();
    void setMaxWidth(QVariant value);
    float maxHeight();
    void setMaxHeight(QVariant value);
    float flex();
    void setFlex(float value);
    float flexBasis();
    void setFlexBasis(QVariant value);
    float flexGrow();
    void setFlexGrow(float value);
    float flexShrink();
    void setFlexShrink(float value);
    QString flexWrap();
    void setFlexWrap(const QString& value);
    float padding();
    void setPadding(QVariant value);
    float paddingTop();
    void setPaddingTop(QVariant value);
    float paddingBottom();
    void setPaddingBottom(QVariant value);
    float paddingLeft();
    void setPaddingLeft(QVariant value);
    float paddingRight();
    void setPaddingRight(QVariant value);
    float paddingHorizontal();
    void setPaddingHorizontal(QVariant value);
    float paddingVertical();
    void setPaddingVertical(QVariant value);
    QString display();
    void setDisplay(const QString& value);
    QString overflow();
    void setOverflow(const QString& value);
    QString position();
    void setPosition(const QString& value);
    QString direction();
    void setDirection(const QString& value);
    bool isDirty();

private:
    bool parsePercents(QVariant& value, float& result);

public slots:
    void markDirty();
    bool isUndefined(float value);

private:
    QScopedPointer<FlexboxPrivate> d_ptr;
};

#endif // FLEXBOX_H
