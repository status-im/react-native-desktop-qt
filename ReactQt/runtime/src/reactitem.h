
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

#ifndef REACTITEM_H
#define REACTITEM_H

#include <QQuickPaintedItem>
#include <QScopedPointer>


class ReactItemPrivate;
class ReactItem : public QQuickPaintedItem
{
  Q_OBJECT
  Q_PROPERTY(QString backfaceVisibility READ backfaceVisibility WRITE setBackfaceVisibility NOTIFY backfaceVisibilityChanged)
  Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
  Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor NOTIFY borderColorChanged)
  Q_PROPERTY(QColor borderTopColor READ borderTopColor WRITE setBorderTopColor NOTIFY borderTopColorChanged)
  Q_PROPERTY(QColor borderRightColor READ borderRightColor WRITE setBorderRightColor NOTIFY borderRightColorChanged)
  Q_PROPERTY(QColor borderBottomColor READ borderColor WRITE setBorderBottomColor NOTIFY borderBottomColorChanged)
  Q_PROPERTY(QColor borderLeftColor READ borderLeftColor WRITE setBorderLeftColor NOTIFY borderLeftColorChanged)
  Q_PROPERTY(double borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged)
  Q_PROPERTY(double borderTopLeftRadius READ borderTopLeftRadius WRITE setBorderTopLeftRadius NOTIFY borderTopLeftRadiusChanged)
  Q_PROPERTY(double borderTopRightRadius READ borderTopRightRadius WRITE setBorderTopRightRadius NOTIFY borderTopRightRadiusChanged)
  Q_PROPERTY(double borderBottomLeftRadius READ borderBottomLeftRadius WRITE setBorderBottomLeftRadius NOTIFY borderBottomLeftRadiusChanged)
  Q_PROPERTY(double borderBottomRightRadius READ borderBottomRightRadius WRITE setBorderBottomRightRadius NOTIFY borderBottomRightRadiusChanged)
  Q_PROPERTY(QString borderStyle READ borderStyle WRITE setBorderStyle NOTIFY borderStyleChanged)
  Q_PROPERTY(double borderWidth READ borderWidth WRITE setBorderWidth NOTIFY borderWidthChanged)
  Q_PROPERTY(double borderTopWidth READ borderTopWidth WRITE setBorderTopWidth NOTIFY borderTopWidthChanged)
  Q_PROPERTY(double borderRightWidth READ borderRightWidth WRITE setBorderRightWidth NOTIFY borderRightWidthChanged)
  Q_PROPERTY(double borderBottomWidth READ borderBottomWidth WRITE setBorderBottomWidth NOTIFY borderBottomWidthChanged)
  Q_PROPERTY(double borderLeftWidth READ borderLeftWidth WRITE setBorderLeftWidth NOTIFY borderLeftWidthChanged)
  Q_PROPERTY(double opacity READ opacity WRITE setOpacity NOTIFY opacityChanged)
  Q_PROPERTY(QString overflow READ overflow WRITE setOverflow NOTIFY overflowChanged)
  Q_PROPERTY(QColor shadowColor READ shadowColor WRITE setShadowColor NOTIFY shadowColorChanged)
  Q_PROPERTY(QSize shadowOffset READ shadowOffset WRITE setShadowOffset NOTIFY shadowOffsetChanged)
  Q_PROPERTY(double shadowOpacity READ shadowOpacity WRITE setShadowOpacity NOTIFY shadowOpacityChanged)
  Q_PROPERTY(double shadowRadius READ shadowRadius WRITE setShadowRadius NOTIFY shadowRadiusChanged)

  Q_DECLARE_PRIVATE(ReactItem)

public:
  ReactItem(QQuickItem *parent = 0);
  ~ReactItem();

  QString backfaceVisibility() const;
  void setBackfaceVisibility(const QString& backfaceVisibility);

  QColor backgroundColor() const;
  void setBackgroundColor(const QColor& backgroundColor);

  QColor borderColor() const;
  void setBorderColor(const QColor& borderColor);

  QColor borderTopColor() const;
  void setBorderTopColor(const QColor& borderTopColor);

  QColor borderRightColor() const;
  void setBorderRightColor(const QColor& borderRightColor);

  QColor borderBottomColor() const;
  void setBorderBottomColor(const QColor& borderBottomColor);

  QColor borderLeftColor() const;
  void setBorderLeftColor(const QColor& borderLeftColor);

  double borderRadius() const;
  void setBorderRadius(double borderRadius);

  double borderTopLeftRadius() const;
  void setBorderTopLeftRadius(double borderTopLeftRadius);

  double borderTopRightRadius() const;
  void setBorderTopRightRadius(double borderTopRightRadius);

  double borderBottomLeftRadius() const;
  void setBorderBottomLeftRadius(double borderBottomLeftRadius);

  double borderBottomRightRadius() const;
  void setBorderBottomRightRadius(double borderBottomRightRadius);

  QString borderStyle() const;
  void setBorderStyle(const QString& borderStyle);

  double borderWidth() const;
  void setBorderWidth(double borderWidth);

  double borderTopWidth() const;
  void setBorderTopWidth(double borderTopWidth);

  double borderRightWidth() const;
  void setBorderRightWidth(double borderRightWidth);

  double borderBottomWidth() const;
  void setBorderBottomWidth(double borderBottomWidth);

  double borderLeftWidth() const;
  void setBorderLeftWidth(double borderLeftWidth);

  double opacity() const;
  void setOpacity(double opacity);

  QString overflow() const;
  void setOverflow(const QString& overflow);

  QColor shadowColor() const;
  void setShadowColor(const QColor& shadowColor);

  QSize shadowOffset() const;
  void setShadowOffset(const QSize& shadowOffset);

  double shadowOpacity() const;
  void setShadowOpacity(double shadowOpacity);

  double shadowRadius() const;
  void setShadowRadius(double shadowRadius);

  static const char* layout_index_property;
Q_SIGNALS:
  void backfaceVisibilityChanged();
  void backgroundColorChanged();
  void borderColorChanged();
  void borderTopColorChanged();
  void borderRightColorChanged();
  void borderBottomColorChanged();
  void borderLeftColorChanged();
  void borderRadiusChanged();
  void borderTopLeftRadiusChanged();
  void borderTopRightRadiusChanged();
  void borderBottomLeftRadiusChanged();
  void borderBottomRightRadiusChanged();
  void borderStyleChanged();
  void borderWidthChanged();
  void borderTopWidthChanged();
  void borderRightWidthChanged();
  void borderBottomWidthChanged();
  void borderLeftWidthChanged();
  void opacityChanged();
  void overflowChanged();
  void shadowColorChanged();
  void shadowOffsetChanged();
  void shadowOpacityChanged();
  void shadowRadiusChanged();

protected:
  void paint(QPainter* painter);
  void updatePolish();

private:
  QScopedPointer<ReactItemPrivate> d_ptr;
};

#endif // REACTITEM_H
