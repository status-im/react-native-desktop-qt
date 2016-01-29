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
  Q_PROPERTY(bool allowFontScaling READ allowFontScaling WRITE setAllowFontScaling NOTIFY allowFontScalingChanged)
  Q_PROPERTY(QString fontFamily READ fontFamily WRITE setFontFamily NOTIFY fontFamilyChanged)
  Q_PROPERTY(double fontSize READ fontSize WRITE setFontSize NOTIFY fontSizeChanged)
  Q_PROPERTY(bool highlighted READ isHighlighted WRITE setHighlighted NOTIFY highlightedChanged)
  Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
  Q_PROPERTY(QString fontStyle READ fontStyle WRITE setFontStyle)
  Q_PROPERTY(QString fontWeight READ fontWeight WRITE setFontWeight)
  Q_PROPERTY(double letterSpacing READ letterSpacing WRITE setLetterSpacing)
  Q_PROPERTY(double lineHeight READ lineHeight WRITE setLineHeight)
  Q_PROPERTY(QString textAlign READ textAlign WRITE setTextAlign)

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

  int numberOfLines() const;
  void setNumberOfLines(int numberOfLines);

  static void polish(QQuickItem* item);
  static ReactTextProperties* get(QQuickItem* item, bool create = true);
  static ReactTextProperties* qmlAttachedProperties(QObject* object);

Q_SIGNALS:
  void allowFontScalingChanged();
  void fontFamilyChanged();
  void fontSizeChanged();
  void highlightedChanged();
  void colorChanged();

private:
  QScopedPointer<ReactTextPropertiesPrivate> d_ptr;
};

QML_DECLARE_TYPEINFO(ReactTextProperties, QML_HAS_ATTACHED_PROPERTIES)

#endif // REACTTEXTPROPERTIES_H
