#ifndef REACTTEXTPROPERTIES_H
#define REACTTEXTPROPERTIES_H

#include <QString>
#include <QColor>
#include <QObject>
#include <QtQml>
#include <QScopedPointer>


class QQuickItem;

class ReactTextPropertiesPrivate;
class ReactTextProperties : public QObject
{
  Q_OBJECT
  Q_PROPERTY(bool allowFontScaling READ allowFontScaling WRITE setAllowFontScaling NOTIFY allowFontScalingChanged)
  Q_PROPERTY(QString fontFamily READ fontFamily WRITE setFontFamily NOTIFY fontFamilyChanged)
  Q_PROPERTY(double fontSize READ fontSize WRITE setFontSize NOTIFY fontSizeChanged)
  Q_PROPERTY(bool highlighted READ isHighlighted WRITE setHighlighted NOTIFY highlightedChanged)
  Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

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
