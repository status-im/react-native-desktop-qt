
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

#include <QPainter>

#include "layout/flexbox.h"
#include "reactitem.h"
#include "utilities.h"

namespace {
Qt::PenStyle borderStyleToPenStyle(const QString& borderStyle) {
    if (borderStyle == "dotted")
        return Qt::DotLine;
    else if (borderStyle == "dashed")
        return Qt::DashLine;

    return Qt::SolidLine;
}
QString penStyleToBorderStyle(Qt::PenStyle penStyle) {
    switch (penStyle) {
    case Qt::SolidLine:
        return "solid";
    case Qt::DotLine:
        return "dotted";
    case Qt::DashLine:
        return "dashed";
    default:
        return "solid";
    }
}
} // namespace

class ReactItemPrivate {
    Q_DECLARE_PUBLIC(ReactItem)
public:
    bool customBorderColors = false;
    bool customBorderWidths = false;
    bool customBorderRadius = false;
    QColor backgroundColor;
    QColor borderColor = Qt::black;
    QColor borderColors[4];
    double borderRadius = 0;
    double borderRadiuses[4] = {
        0,
    };
    double borderWidth = 0;
    double borderWidths[4] = {
        0,
    };
    Qt::PenStyle borderStyle = Qt::SolidLine;
    QRectF outerRects[4];
    QRectF innerRects[4];
    QVector<float> transform;

    ReactItemPrivate(ReactItem* q) : q_ptr(q) {}

    void calculateCornerRects(const QRectF& otr, const QRectF& inr) {
        outerRects[0] = QRectF(otr.topLeft(), QSizeF(borderRadiuses[0] * 2, borderRadiuses[0] * 2));
        outerRects[1] = QRectF(otr.topRight().x() - borderRadiuses[1] * 2,
                               otr.topRight().y(),
                               borderRadiuses[1] * 2,
                               borderRadiuses[1] * 2);
        outerRects[2] = QRectF(otr.bottomRight().x() - borderRadiuses[2] * 2,
                               otr.bottomRight().y() - borderRadiuses[2] * 2,
                               borderRadiuses[2] * 2,
                               borderRadiuses[2] * 2);
        outerRects[3] = QRectF(otr.bottomLeft().x(),
                               otr.bottomLeft().y() - borderRadiuses[3] * 2,
                               borderRadiuses[3] * 2,
                               borderRadiuses[3] * 2);

        innerRects[0] =
            QRectF(inr.topLeft(),
                   QSizeF((borderRadiuses[0] - borderWidths[3]) * 2, (borderRadiuses[0] - borderWidths[0]) * 2));
        innerRects[1] = QRectF(inr.topRight().x() - (borderRadiuses[1] - borderWidths[1]) * 2,
                               inr.topRight().y(),
                               (borderRadiuses[1] - borderWidths[1]) * 2,
                               (borderRadiuses[1] - borderWidths[0]) * 2);
        innerRects[2] = QRectF(inr.bottomRight().x() - (borderRadiuses[2] - borderWidths[1]) * 2,
                               inr.bottomRight().y() - (borderRadiuses[2] - borderWidths[2]) * 2,
                               (borderRadiuses[2] - borderWidths[1]) * 2,
                               (borderRadiuses[2] - borderWidths[2]) * 2);
        innerRects[3] = QRectF(inr.bottomLeft().x(),
                               inr.bottomLeft().y() - (borderRadiuses[3] - borderWidths[2]) * 2,
                               (borderRadiuses[3] - borderWidths[3]) * 2,
                               (borderRadiuses[3] - borderWidths[2]) * 2);
    }

    ReactItem* q_ptr;
};

QString ReactItem::backfaceVisibility() const {
    return QString();
}

void ReactItem::setBackfaceVisibility(const QString& backfaceVisibility) {}

QColor ReactItem::backgroundColor() const {
    return d_func()->backgroundColor;
}

void ReactItem::setBackgroundColor(const QColor& backgroundColor) {
    Q_D(ReactItem);
    if (d->backgroundColor == backgroundColor)
        return;
    d->backgroundColor = backgroundColor;
    update();
    Q_EMIT backgroundColorChanged();
}

QColor ReactItem::borderColor() const {
    return d_func()->borderColor;
}

void ReactItem::setBorderColor(const QColor& borderColor) {
    Q_D(ReactItem);
    if (d->borderColor == borderColor)
        return;
    d->borderColor = borderColor;
    for (int i = 0; i < 4; ++i) {
        d->borderColors[i] = borderColor;
    }
    d->customBorderColors = false;
    update();
    Q_EMIT borderColorChanged();
}

QColor ReactItem::borderTopColor() const {
    return d_func()->borderColors[0];
}

void ReactItem::setBorderTopColor(const QColor& borderTopColor) {
    Q_D(ReactItem);
    if (d->borderColors[0] == borderTopColor)
        return;
    d->borderColors[0] = borderTopColor;
    d->customBorderColors = true;
    update();
    Q_EMIT borderTopColorChanged();
}

QColor ReactItem::borderRightColor() const {
    return d_func()->borderColors[1];
}

void ReactItem::setBorderRightColor(const QColor& borderRightColor) {
    Q_D(ReactItem);
    if (d->borderColors[1] == borderRightColor)
        return;
    d->borderColors[1] = borderRightColor;
    d->customBorderColors = true;
    update();
    Q_EMIT borderRightColorChanged();
}

QColor ReactItem::borderBottomColor() const {
    return d_func()->borderColors[2];
}

void ReactItem::setBorderBottomColor(const QColor& borderBottomColor) {
    Q_D(ReactItem);
    if (d->borderColors[2] == borderBottomColor)
        return;
    d->borderColors[2] = borderBottomColor;
    d->customBorderColors = true;
    update();
    Q_EMIT borderBottomColorChanged();
}

QColor ReactItem::borderLeftColor() const {
    return d_func()->borderColors[3];
}

void ReactItem::setBorderLeftColor(const QColor& borderLeftColor) {
    Q_D(ReactItem);
    if (d->borderColors[3] == borderLeftColor)
        return;
    d->borderColors[3] = borderLeftColor;
    d->customBorderColors = true;
    update();
    Q_EMIT borderLeftColorChanged();
}

double ReactItem::borderRadius() const {
    return d_func()->borderRadius;
}

void ReactItem::setBorderRadius(double borderRadius) {
    Q_D(ReactItem);
    if (qFuzzyCompare(d->borderRadius, borderRadius))
        return;
    d->borderRadius = borderRadius;
    for (int i = 0; i < 4; ++i) {
        d->borderRadiuses[i] = borderRadius;
    }
    d->customBorderRadius = false;
    update();
    Q_EMIT borderRadiusChanged();
}

double ReactItem::borderTopLeftRadius() const {
    return d_func()->borderRadiuses[0];
}

void ReactItem::setBorderTopLeftRadius(double borderTopLeftRadius) {
    Q_D(ReactItem);
    if (qFuzzyCompare(d->borderRadiuses[0], borderTopLeftRadius))
        return;
    d->borderRadiuses[0] = borderTopLeftRadius;
    d->customBorderRadius = true;
    update();
    Q_EMIT borderTopLeftRadiusChanged();
}

double ReactItem::borderTopRightRadius() const {
    return d_func()->borderRadiuses[1];
}

void ReactItem::setBorderTopRightRadius(double borderTopRightRadius) {
    Q_D(ReactItem);
    if (qFuzzyCompare(d->borderRadiuses[1], borderTopRightRadius))
        return;
    d->borderRadiuses[1] = borderTopRightRadius;
    d->customBorderRadius = true;
    update();
    Q_EMIT borderTopRightRadiusChanged();
}

double ReactItem::borderBottomLeftRadius() const {
    return d_func()->borderRadiuses[3];
}

void ReactItem::setBorderBottomLeftRadius(double borderBottomLeftRadius) {
    Q_D(ReactItem);
    if (qFuzzyCompare(d->borderRadiuses[3], borderBottomLeftRadius))
        return;
    d->borderRadiuses[3] = borderBottomLeftRadius;
    d->customBorderRadius = true;
    update();
    Q_EMIT borderBottomLeftRadiusChanged();
}

double ReactItem::borderBottomRightRadius() const {
    return d_func()->borderRadiuses[2];
}

void ReactItem::setBorderBottomRightRadius(double borderBottomRightRadius) {
    Q_D(ReactItem);
    if (qFuzzyCompare(d->borderRadiuses[2], borderBottomRightRadius))
        return;
    d->borderRadiuses[2] = borderBottomRightRadius;
    d->customBorderRadius = true;
    update();
    Q_EMIT borderBottomRightRadiusChanged();
}

QString ReactItem::borderStyle() const {
    return penStyleToBorderStyle(d_func()->borderStyle);
}

void ReactItem::setBorderStyle(const QString& borderStyle) {
    Q_D(ReactItem);
    Qt::PenStyle bs = borderStyleToPenStyle(borderStyle);
    if (d->borderStyle == bs)
        return;
    d->borderStyle = bs;
    update();
    Q_EMIT borderStyleChanged();
}

double ReactItem::borderWidth() const {
    return d_func()->borderWidth;
}

void ReactItem::setBorderWidth(double borderWidth) {
    Q_D(ReactItem);
    if (qFuzzyCompare(d->borderWidth, borderWidth))
        return;
    d->borderWidth = borderWidth;
    for (int i = 0; i < 4; ++i) {
        d->borderWidths[i] = borderWidth;
    }
    d->customBorderWidths = false;
    update();
    Q_EMIT borderWidthChanged();
}

double ReactItem::borderTopWidth() const {
    return d_func()->borderWidths[0];
}

void ReactItem::setBorderTopWidth(double borderTopWidth) {
    Q_D(ReactItem);
    if (qFuzzyCompare(d->borderWidths[0], borderTopWidth))
        return;
    d->borderWidths[0] = borderTopWidth;
    d->customBorderWidths = true;
    update();
    Q_EMIT borderTopWidthChanged();
}

double ReactItem::borderRightWidth() const {
    return d_func()->borderWidths[1];
}

void ReactItem::setBorderRightWidth(double borderRightWidth) {
    Q_D(ReactItem);
    if (qFuzzyCompare(d->borderWidths[1], borderRightWidth))
        return;
    d->borderWidths[1] = borderRightWidth;
    d->customBorderWidths = true;
    update();
    Q_EMIT borderRightWidthChanged();
}

double ReactItem::borderBottomWidth() const {
    return d_func()->borderWidths[2];
}

void ReactItem::setBorderBottomWidth(double borderBottomWidth) {
    Q_D(ReactItem);
    if (qFuzzyCompare(d->borderWidths[2], borderBottomWidth))
        return;
    d->borderWidths[2] = borderBottomWidth;
    d->customBorderWidths = true;
    update();
    Q_EMIT borderBottomWidthChanged();
}

double ReactItem::borderLeftWidth() const {
    return d_func()->borderWidths[3];
}

void ReactItem::setBorderLeftWidth(double borderLeftWidth) {
    Q_D(ReactItem);
    if (qFuzzyCompare(d->borderWidths[3], borderLeftWidth))
        return;
    d->borderWidths[3] = borderLeftWidth;
    d->customBorderWidths = true;
    update();
    Q_EMIT borderLeftWidthChanged();
}

double ReactItem::opacity() const {
    return QQuickItem::opacity();
}

void ReactItem::setOpacity(double opacity) {
    QQuickItem::setOpacity(opacity);
}

QString ReactItem::overflow() const {
    return clip() ? "hidden" : "visible";
}

void ReactItem::setOverflow(const QString& overflow) {
    const bool clip = overflow == "hidden";
    if (QQuickItem::clip() == clip)
        return;
    setClip(overflow == "hidden");
    Q_EMIT overflowChanged();
}

QColor ReactItem::shadowColor() const {
    return QColor();
}

void ReactItem::setShadowColor(const QColor& shadowColor) {}

QSize ReactItem::shadowOffset() const {
    return QSize();
}

void ReactItem::setShadowOffset(const QSize& shadowOffset) {}

double ReactItem::shadowOpacity() const {
    return 1;
}

void ReactItem::setShadowOpacity(double shadowOpacity) {}

double ReactItem::shadowRadius() const {
    return 0;
}

void ReactItem::setShadowRadius(double shadowRadius) {}

QVector<float> ReactItem::transform() const {
    return d_func()->transform;
}

void ReactItem::setTransform(QVector<float>& transform) {
    Q_D(ReactItem);
    d->transform = transform;
    QQmlListReference r(this, "transform");
    r.clear();
    r.append(new utilities::MatrixTransform(transform, this));
}

ReactItem::ReactItem(QQuickItem* parent) : QQuickPaintedItem(parent), d_ptr(new ReactItemPrivate(this)) {
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
}

ReactItem::~ReactItem() {}

void ReactItem::paint(QPainter* painter) {
    Q_D(ReactItem);

    painter->setRenderHint(QPainter::Antialiasing);

    QRectF area(QPointF(0, 0), QSizeF(width(), height()));

    // TODO; support variant widths with variant radii
    QRectF border = area.adjusted(d->borderWidths[3], d->borderWidths[0], -d->borderWidths[1], -d->borderWidths[2]);

    if (border != area) {
        if (d->customBorderWidths || d->customBorderColors || d->customBorderRadius) {
            if (!d->customBorderColors && !(d->borderRadius > 0 || d->customBorderRadius)) {
                QPainterPath path;
                path.addRect(area);
                path.addRect(border);
                painter->fillPath(path, d->borderColor);
                if (d->backgroundColor.isValid() && d->backgroundColor.alpha() != 0) {
                    QPainterPath ap;
                    ap.addRect(area);
                    painter->fillPath(ap.subtracted(path), d->backgroundColor);
                }
            } else {
                if (d->backgroundColor.isValid() && d->backgroundColor.alpha() != 0) {
                    QPainterPath backgroundPath;
                    backgroundPath.addRect(area);
                    painter->fillPath(backgroundPath, d->backgroundColor);
                }

                d->calculateCornerRects(area, border);
                QPainterPath path;
                // TODO: background

                // top
                if (d->borderRadiuses[0] > 0) {
                    if (!d->innerRects[0].isEmpty()) {
                        path.arcMoveTo(d->innerRects[0], 90);
                        path.arcTo(d->innerRects[0], 90, 45);
                    } else {
                        path.moveTo(border.topLeft());
                    }
                    path.arcTo(d->outerRects[0], -225, -45);
                } else {
                    path.moveTo(border.topLeft());
                    path.lineTo(area.topLeft());
                }
                if (d->borderRadiuses[1] > 0) {
                    path.arcTo(d->outerRects[1], -270, -45);
                    if (!d->innerRects[1].isEmpty()) {
                        path.arcTo(d->innerRects[1], 45, 45);
                    } else {
                        path.lineTo(border.topRight());
                    }
                } else {
                    path.lineTo(area.topRight());
                    path.lineTo(border.topRight());
                }
                path.closeSubpath();
                painter->fillPath(path, d->borderColors[0]);

                // right
                path = QPainterPath();
                if (d->borderRadiuses[1] > 0) {
                    if (!d->innerRects[1].isEmpty()) {
                        path.arcMoveTo(d->innerRects[1], 0);
                        path.arcTo(d->innerRects[1], 0, 45);
                    } else {
                        path.moveTo(border.topRight());
                    }
                    path.arcTo(d->outerRects[1], 45, -45);
                } else {
                    path.moveTo(border.topRight());
                    path.lineTo(area.topRight());
                }
                if (d->borderRadiuses[2] > 0) {
                    path.arcTo(d->outerRects[2], 0, -45);
                    if (!d->innerRects[2].isEmpty()) {
                        path.arcTo(d->innerRects[2], -45, 45);
                    } else {
                        path.lineTo(border.bottomRight());
                    }
                } else {
                    path.lineTo(area.bottomRight());
                    path.lineTo(border.bottomRight());
                }
                path.closeSubpath();
                painter->fillPath(path, d->borderColors[1]);

                // bottom
                path = QPainterPath();
                if (d->borderRadiuses[2] > 0) {
                    if (!d->innerRects[2].isEmpty()) {
                        path.arcMoveTo(d->innerRects[2], -90);
                        path.arcTo(d->innerRects[2], -90, 45);
                    } else {
                        path.moveTo(border.bottomRight());
                    }
                    path.arcTo(d->outerRects[2], -45, -45);
                } else {
                    path.moveTo(border.bottomRight());
                    path.lineTo(area.bottomRight());
                }
                if (d->borderRadiuses[3] > 0) {
                    path.arcTo(d->outerRects[3], -90, -45);
                    if (!d->innerRects[3].isEmpty()) {
                        path.arcTo(d->innerRects[3], -135, -45);
                    } else {
                        path.lineTo(border.bottomLeft());
                    }
                } else {
                    path.lineTo(area.bottomLeft());
                    path.lineTo(border.bottomLeft());
                }
                path.closeSubpath();
                painter->fillPath(path, d->borderColors[2]);

                // left
                path = QPainterPath();
                if (d->borderRadiuses[3] > 0) {
                    if (!d->innerRects[3].isEmpty()) {
                        path.arcMoveTo(d->innerRects[3], -180);
                        path.arcTo(d->innerRects[3], -180, 45);
                    } else {
                        path.moveTo(border.bottomLeft());
                    }
                    path.arcTo(d->outerRects[3], -135, -45);
                } else {
                    path.moveTo(border.bottomLeft());
                    path.lineTo(area.bottomLeft());
                }
                if (d->borderRadiuses[0] > 0) {
                    path.arcTo(d->outerRects[0], -180, -45);
                    if (!d->innerRects[0].isEmpty()) {
                        path.arcTo(d->innerRects[0], -225, 45);
                    } else {
                        path.lineTo(border.topLeft());
                    }
                } else {
                    path.lineTo(area.topLeft());
                    path.lineTo(border.topLeft());
                }
                path.closeSubpath();
                painter->fillPath(path, d->borderColors[3]);
            }
        } else {
            QPainterPath path;
            path.addRoundedRect(border, d->borderRadius, d->borderRadius);
            if (d->backgroundColor.isValid() && d->backgroundColor.alpha() != 0) {
                painter->fillPath(path, d->backgroundColor);
            }
            if (d->borderColor.alpha() != 0) {
                QPen pen(d->borderStyle);
                pen.setColor(d->borderColor);
                pen.setWidthF(d->borderWidth);
                pen.setJoinStyle(Qt::MiterJoin);
                painter->setPen(pen);
                painter->drawPath(path);
            }
        }
    } else {
        if (d->backgroundColor.isValid() && d->backgroundColor.alpha() != 0) {
            QPainterPath path;
            path.addRoundedRect(area, d->borderRadius, d->borderRadius);
            painter->fillPath(path, d->backgroundColor);
        }
    }
}
