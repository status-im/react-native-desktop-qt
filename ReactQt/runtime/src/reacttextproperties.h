
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

#ifndef REACTTEXTPROPERTIES_H
#define REACTTEXTPROPERTIES_H

#include <QString>
#include <QColor>
#include <QObject>
#include <QtQml>
#include <QScopedPointer>

#include "reactpropertyhandler.h"


class QQuickItem;

class ReactTextPropertiesPrivate;
class ReactTextProperties : public ReactPropertyHandler
{
  Q_OBJECT
  Q_PROPERTY(bool allowFontScaling READ allowFontScaling WRITE setAllowFontScaling)
  Q_PROPERTY(QString fontFamily READ fontFamily WRITE setFontFamily)
  Q_PROPERTY(double fontSize READ fontSize WRITE setFontSize)
  Q_PROPERTY(bool highlighted READ isHighlighted WRITE setHighlighted)
  Q_PROPERTY(QColor color READ color WRITE setColor)
  Q_PROPERTY(QString fontStyle READ fontStyle WRITE setFontStyle)
  Q_PROPERTY(QString fontWeight READ fontWeight WRITE setFontWeight)
  Q_PROPERTY(double letterSpacing READ letterSpacing WRITE setLetterSpacing)
  Q_PROPERTY(double lineHeight READ lineHeight WRITE setLineHeight)
  Q_PROPERTY(QString textAlign READ textAlign WRITE setTextAlign)
  Q_PROPERTY(QString textDecorationLine READ textDecorationLine WRITE setTextDecorationLine)
  Q_PROPERTY(QString textDecorationStyle READ textDecorationStyle WRITE setTextDecorationStyle)
  Q_PROPERTY(QColor textDecorationColor READ textDecorationColor WRITE setTextDecorationColor)
  Q_PROPERTY(QString writingDirection READ writingDirection WRITE setWritingDirection)

  Q_DECLARE_PRIVATE(ReactTextProperties);

public:
  ReactTextProperties(QObject* parent = 0);
  ~ReactTextProperties();

  bool allowFontScaling() const;
  void setAllowFontScaling(bool allowFontScaling);

  QString fontFamily() const;
  void setFontFamily(const QString& fontFamily);

  double fontSize() const;
  void setFontSize(double fontSize);

  bool isHighlighted() const;
  void setHighlighted(bool highlighted);

  QColor color() const;
  void setColor(const QColor& color);

  QString fontStyle() const;
  void setFontStyle(const QString& fontStyle);

  QString fontWeight();
  void setFontWeight(const QString& fontWeight);

  double letterSpacing();
  void setLetterSpacing(double letterSpacing);

  double lineHeight() const;
  void setLineHeight(double lineHeight);

  QString textAlign() const;
  void setTextAlign(const QString& textAlign);

  QString textDecorationLine() const;
  void setTextDecorationLine(const QString& textDecorationLine);

  QString textDecorationStyle() const;
  void setTextDecorationStyle(const QString& textDecorationStyle);

  QColor textDecorationColor() const;
  void setTextDecorationColor(const QColor& textDecorationColor);

  QString writingDirection() const;
  void setWritingDirection(const QString& writingDirection);

  int numberOfLines() const;
  void setNumberOfLines(int numberOfLines);

  void setDirty(bool dirty);

  void hookLayout();

  static ReactTextProperties* get(QQuickItem* item, bool create = true);
  static ReactTextProperties* qmlAttachedProperties(QObject* object);
  static QString decorateTextFromProperties(const QString& text, const QVariantMap& properties);

private:
  QScopedPointer<ReactTextPropertiesPrivate> d_ptr;
};

QML_DECLARE_TYPEINFO(ReactTextProperties, QML_HAS_ATTACHED_PROPERTIES)

#endif // REACTTEXTPROPERTIES_H
