
#include <QQuickItem>

#include "reacttextproperties.h"


class ReactTextPropertiesPrivate
{
public:
  bool dirty;
  bool allowFontScaling;
  QString fontFamily;
  double fontSize;
  bool highlighted;
  QColor color;
  QString fontStyle;
  QString fontWeight;
  double letterSpacing;
  double lineHeight;
  QString textAlign;
  int numberOfLines;
  QQuickItem* item;

  void apply() {
    if (!dirty)
      return;
    applyTextProperties(item, this);
    dirty = false;
  }

  static ReactTextPropertiesPrivate* get(ReactTextProperties* rtp) {
    return rtp->d_func();
  }

private:
  void applyTextProperties(QQuickItem* item, ReactTextPropertiesPrivate* rtp)
  {
    // XXX: only set dirty properties
    if (rtp->fontSize != -1) {
      QQmlProperty p(item, "font.pointSize");
      p.write(QVariant::fromValue(rtp->fontSize));
    }

    if (!rtp->fontFamily.isEmpty()) {
      QQmlProperty p(item, "font.family");
      p.write(QVariant::fromValue(rtp->fontFamily));
    }

    if (rtp->color.isValid())
      item->setProperty("color", rtp->color);

    if (!rtp->textAlign.isEmpty()) {
      if (textAlign == "auto" || textAlign == "center") {
        item->setProperty("horizontalAlignment", Qt::AlignHCenter);
      } else if (textAlign == "left") {
        item->setProperty("horizontalAlignment", Qt::AlignLeft);
      } else if (textAlign == "right") {
        item->setProperty("horizontalAlignment", Qt::AlignRight);
      } else if (textAlign == "justify") {
        item->setProperty("horizontalAlignment", Qt::AlignJustify);
      }
    }

    for (auto c : item->childItems())
      applyTextProperties(c, rtp);
  }
};


ReactTextProperties::ReactTextProperties(QObject* parent)
  : ReactPropertyHandler(parent)
  , d_ptr(new ReactTextPropertiesPrivate)
{
  Q_D(ReactTextProperties);
  d->dirty = false;
  d->allowFontScaling = false;
  d->fontSize = -1;
  d->highlighted = false;
  d->numberOfLines = -1;
  d->item = qobject_cast<QQuickItem*>(parent);
  if (d->item == nullptr) {
    qCritical() << "ReactTextProperties only applies to visual items";
  }
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
  d->dirty = true;
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
  d->dirty = true;
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
  d->dirty = true;
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
  d->dirty = true;
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
  d->dirty = true;
  Q_EMIT colorChanged();
}

QString ReactTextProperties::fontStyle() const
{
  return d_func()->fontStyle;
}

void ReactTextProperties::setFontStyle(const QString& fontStyle)
{
  Q_D(ReactTextProperties);
  if (d->fontStyle == fontStyle)
    return;
  d->fontStyle = fontStyle;
  d->dirty = true;
}

QString ReactTextProperties::fontWeight()
{
  return d_func()->fontWeight;
}

void ReactTextProperties::setFontWeight(const QString& fontWeight)
{
  Q_D(ReactTextProperties);
  if (d->fontWeight == fontWeight)
    return;
  d->fontWeight = fontWeight;
  d->dirty = true;
}

double ReactTextProperties::letterSpacing()
{
  return d_func()->letterSpacing;
}

void ReactTextProperties::setLetterSpacing(double letterSpacing)
{
  Q_D(ReactTextProperties);
  if (d->letterSpacing == letterSpacing)
    return;
  d->letterSpacing = letterSpacing;
  d->dirty = true;
}

double ReactTextProperties::lineHeight() const
{
  return d_func()->lineHeight;
}

void ReactTextProperties::setLineHeight(double lineHeight)
{
  Q_D(ReactTextProperties);
  if (d->lineHeight == lineHeight)
    return;
  d->lineHeight = lineHeight;
  d->dirty = true;
}

QString ReactTextProperties::textAlign() const
{
  return d_func()->textAlign;
}

void ReactTextProperties::setTextAlign(const QString& textAlign)
{
  Q_D(ReactTextProperties);
  if (d->textAlign == textAlign)
    return;
  d->textAlign = textAlign;
  d->dirty = true;
}

int ReactTextProperties::numberOfLines() const
{
  return d_func()->numberOfLines;
}

void ReactTextProperties::setNumberOfLines(int numberOfLines)
{
  Q_D(ReactTextProperties);
  if (d->numberOfLines == numberOfLines)
    return;
  d->numberOfLines = numberOfLines;
}

void ReactTextProperties::polish(QQuickItem* item)
{
  ReactTextProperties* rtp = ReactTextProperties::get(item, false);
  if (rtp != nullptr) {
    // apply text properties to all children
    ReactTextPropertiesPrivate::get(rtp)->apply();
  }

  // Keep searching
  for (auto c : item->childItems()) {
    polish(c);
  }
}

ReactTextProperties* ReactTextProperties::get(QQuickItem* item, bool create)
{
  return qobject_cast<ReactTextProperties*>(qmlAttachedPropertiesObject<ReactTextProperties>(item, create));
}

ReactTextProperties* ReactTextProperties::qmlAttachedProperties(QObject* object)
{
  return new ReactTextProperties(object);
}
