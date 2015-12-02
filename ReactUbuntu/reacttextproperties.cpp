
#include <QQuickItem>

#include "reacttextproperties.h"


class ReactTextPropertiesPrivate
{
public:
  bool allowFontScaling;
  QString fontFamily;
  double fontSize;
  bool highlighted;
  QColor color;
};


ReactTextProperties::ReactTextProperties(QObject* parent)
  : QObject(parent)
  , d_ptr(new ReactTextPropertiesPrivate)
{
}

ReactTextProperties::~ReactTextProperties()
{
}

bool ReactTextProperties::allowFontScaling() const
{
  return d_func()->allowFontScaling;
}

void ReactTextProperties::setAllowFontScaling(bool allowFontScaling)
{
  Q_D(ReactTextProperties);
  if (d->allowFontScaling == allowFontScaling)
    return;
  d->allowFontScaling = allowFontScaling;
  Q_EMIT allowFontScalingChanged();
}

QString ReactTextProperties::fontFamily() const
{
  return d_func()->fontFamily;
}

void ReactTextProperties::setFontFamily(const QString& fontFamily)
{
  Q_D(ReactTextProperties);
  if (d->fontFamily == fontFamily)
    return;
  d->fontFamily = fontFamily;
  Q_EMIT fontFamilyChanged();
}

double ReactTextProperties::fontSize() const
{
  return d_func()->fontSize;
}

void ReactTextProperties::setFontSize(double fontSize)
{
  Q_D(ReactTextProperties);
  if (d->fontSize == fontSize) // XXX:
    return;
  d->fontSize = fontSize;
  Q_EMIT fontSizeChanged();
}

bool ReactTextProperties::isHighlighted() const
{
  return d_func()->highlighted;
}

void ReactTextProperties::setHighlighted(bool highlighted)
{
  Q_D(ReactTextProperties);
  if (d->highlighted == highlighted)
    return;
  d->highlighted = highlighted;
  Q_EMIT highlightedChanged();
}

QColor ReactTextProperties::color() const
{
  return d_func()->color;
}

void ReactTextProperties::setColor(const QColor& color)
{
  Q_D(ReactTextProperties);
  if (d->color == color)
    return;
  d->color = color;
  Q_EMIT colorChanged();
}

ReactTextProperties* ReactTextProperties::get(QQuickItem* item, bool create)
{
  return qobject_cast<ReactTextProperties*>(qmlAttachedPropertiesObject<ReactTextProperties>(item, create));
}

ReactTextProperties* ReactTextProperties::qmlAttachedProperties(QObject* object)
{
  return new ReactTextProperties(object);
}
