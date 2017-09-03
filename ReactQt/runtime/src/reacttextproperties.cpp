
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

#include <map>

#include <QQuickItem>

#include <QDebug>

#include "reacttextproperties.h"
#include "reactrawtextproperties.h"
#include "reactflexlayout.h"


namespace {
const char* k_allow_font_scaling = "allowFontScaling";
const char* k_font_family = "fontFamily";
const char* k_font_size = "fontSize";
const char* k_highlighted = "highlighted";
const char* k_color = "color";
const char* k_font_style = "fontStyle";
const char* k_font_weight = "fontWeight";
const char* k_letter_spacing = "letterSpacing";
const char* k_line_height = "lineHeight";
const char* k_text_align = "textAlign";
const char* k_text_decoration_line = "textDecorationLine";
const char* k_text_decoration_style = "textDecorationStyle";
const char* k_text_decoration_color = "textDecorationColor";
const char* k_writing_direction = "textDecorationColor";
const char* k_number_of_lines = "numberOfLines";
}

class ReactTextPropertiesPrivate
{
public:
  typedef std::map<QString, QVariant> property_map;
  bool dirty;
  property_map properties;
  QQuickItem* item;

  void setupLayout() {
    ReactFlexLayout* fl = ReactFlexLayout::get(item);
    fl->setMeasureFunction([=](double width, FlexMeasureMode widthMode, double height, FlexMeasureMode heightMode) {
      if (dirty) {
        QString ts = textWithProperties(property_map());
        qDebug()<<"========== TEXT =============";
        qDebug()<<ts;
        item->setProperty("text", ts);
        dirty = false;
      }
      double contentWidth = item->property("contentWidth").value<double>();
      double sw = 0;
      if (std::isnan(width)) {
        sw = contentWidth;
      } else {
        sw = contentWidth == 0 ? width : qMin(contentWidth, width);
      }

      item->setWidth(sw);

      return std::make_pair(float(sw),
                            item->property("contentHeight").value<float>());
    });
  }

  template<typename VT>
  VT value(const char* key, const VT& defaulValue = VT()) const {
    property_map::const_iterator it = properties.find(key);
    if (it == properties.end())
      return defaulValue;
    return (*it).second.value<VT>();
  }

  template<typename VT>
  void setValue(const char* key, const VT& value) {
    properties[key] = value;
    setDirty(true);
  }

  QString textAlign() const {
    QQmlProperty p(item, "horizontalAlignment");
    switch (p.read().toInt()) {
    case Qt::AlignLeft: return "left";
    case Qt::AlignRight: return "right";
    case Qt::AlignHCenter: return "center";
    case Qt::AlignJustify: return "justify";
    }
  }

  void setTextAlign(const QString& textAlign) {
    QQmlProperty p(item, "horizontalAlignment");
    if (textAlign == "left") {
      p.write(Qt::AlignLeft);
    } else if (textAlign == "right") {
      p.write(Qt::AlignRight);
    } else if (textAlign == "center") {
      p.write(Qt::AlignHCenter);
    } else if (textAlign == "justify") {
      p.write(Qt::AlignJustify);
    }
  }

  void setDirty(bool dirty) {
    this->dirty = dirty;
    ReactTextProperties* tp = ReactTextProperties::get(item->parentItem(), false);
    if (tp != nullptr) {
      tp->setDirty(dirty);
    }
    ReactFlexLayout::get(item)->setDirty(dirty);
  }

  QString textWithProperties(const property_map& properties) {
    property_map mp = this->properties;
    mp.insert(properties.begin(), properties.end());

    QString text;
    for (auto& c : item->childItems()) {
      ReactRawTextProperties* tp = ReactRawTextProperties::get(c, false);
      if (tp != nullptr) {
        text += ReactTextProperties::decorateTextFromProperties(tp->text(), QMap<QString, QVariant>(mp));
      } else {
        text += ReactTextPropertiesPrivate::get(ReactTextProperties::get(c))->textWithProperties(mp);
      }
    }
    return text;
  }

  static ReactTextPropertiesPrivate* get(ReactTextProperties* tp) {
    return tp->d_func();
  }
};


ReactTextProperties::ReactTextProperties(QObject* parent)
  : ReactPropertyHandler(parent)
  , d_ptr(new ReactTextPropertiesPrivate)
{
  Q_D(ReactTextProperties);
  d->dirty = false;
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
  return d_func()->value<bool>(k_allow_font_scaling);
}

void ReactTextProperties::setAllowFontScaling(bool allowFontScaling)
{
  Q_D(ReactTextProperties);
  d->item->setProperty("fontSizeMode", allowFontScaling ? 3 : 0);// XXX:
  d->setDirty(true);
}

QString ReactTextProperties::fontFamily() const
{
  return d_func()->value<QString>(k_font_family);
}

void ReactTextProperties::setFontFamily(const QString& fontFamily)
{
  d_func()->setValue(k_font_family, fontFamily);
}

double ReactTextProperties::fontSize() const
{
  return d_func()->value<double>(k_font_size, -1);
}

void ReactTextProperties::setFontSize(double fontSize)
{
  d_func()->setValue(k_font_size, fontSize);
}

bool ReactTextProperties::isHighlighted() const
{
  return d_func()->value<bool>(k_highlighted);
}

void ReactTextProperties::setHighlighted(bool isHighlighted)
{
  d_func()->setValue(k_highlighted, isHighlighted);
}

QColor ReactTextProperties::color() const
{
  return d_func()->value<QColor>(k_color);
}

void ReactTextProperties::setColor(const QColor& color)
{
  d_func()->setValue(k_color, color);
}

QString ReactTextProperties::fontStyle() const
{
  return d_func()->value<QString>(k_font_style);
}

void ReactTextProperties::setFontStyle(const QString& fontStyle)
{
  d_func()->setValue(k_font_style, fontStyle);
}

QString ReactTextProperties::fontWeight()
{
  return d_func()->value<QString>(k_font_weight);
}

void ReactTextProperties::setFontWeight(const QString& fontWeight)
{
  d_func()->setValue(k_font_weight, fontWeight);
}

double ReactTextProperties::letterSpacing()
{
  return d_func()->value<double>(k_letter_spacing);
}

void ReactTextProperties::setLetterSpacing(double letterSpacing)
{
  d_func()->setValue(k_letter_spacing, letterSpacing);
}

double ReactTextProperties::lineHeight() const
{
  return d_func()->value<double>(k_line_height, -1);
}

void ReactTextProperties::setLineHeight(double lineHeight)
{
  d_func()->setValue(k_line_height, lineHeight);
}

QString ReactTextProperties::textAlign() const
{
  return d_func()->textAlign();
}

void ReactTextProperties::setTextAlign(const QString& textAlign)
{
  d_func()->setTextAlign(textAlign);
}

QString ReactTextProperties::textDecorationLine() const
{
  return d_func()->value<QString>(k_text_decoration_line);
}

void ReactTextProperties::setTextDecorationLine(const QString& textDecorationLine)
{
  d_func()->setValue(k_text_decoration_line, textDecorationLine);
}

QString ReactTextProperties::textDecorationStyle() const
{
  return d_func()->value<QString>(k_text_decoration_style);
}

void ReactTextProperties::setTextDecorationStyle(const QString& textDecorationStyle)
{
  d_func()->setValue(k_text_decoration_style, textDecorationStyle);
}

QColor ReactTextProperties::textDecorationColor() const
{
  return d_func()->value<QString>(k_text_decoration_color);
}

void ReactTextProperties::setTextDecorationColor(const QColor& textDecorationColor)
{
  d_func()->setValue(k_text_decoration_style, textDecorationColor);
}

QString ReactTextProperties::writingDirection() const
{
  return d_func()->value<QString>(k_writing_direction);
}

void ReactTextProperties::setWritingDirection(const QString& writingDirection)
{
  d_func()->setValue(k_writing_direction, writingDirection);
}

int ReactTextProperties::numberOfLines() const
{
  return d_func()->value<int>(k_number_of_lines, -1);
}

void ReactTextProperties::setNumberOfLines(int numberOfLines)
{
  Q_D(ReactTextProperties);
  d->item->setProperty("maxiumLineCount", numberOfLines);
  d->setDirty(true);
}

void ReactTextProperties::setDirty(bool dirty)
{
  d_func()->setDirty(dirty);
}

void ReactTextProperties::hookLayout()
{
  d_func()->setupLayout();
}

ReactTextProperties* ReactTextProperties::get(QQuickItem* item, bool create)
{
  return qobject_cast<ReactTextProperties*>(qmlAttachedPropertiesObject<ReactTextProperties>(item, create));
}

ReactTextProperties* ReactTextProperties::qmlAttachedProperties(QObject* object)
{
  return new ReactTextProperties(object);
}


QString ReactTextProperties::decorateTextFromProperties(const QString& text, const QVariantMap& properties)
{
  QString decoratedText = "<span style=\"";

  for (const QString& key : properties.keys()) {
    QString value = properties.value(key).value<QString>();
    if (key == "fontFamily") {
      decoratedText += QString("font-family:%1;").arg(value);
    } else if (key == "fontSize") {
      decoratedText += QString("font-size:%1pt;").arg(value);
    } else if (key == "color") {
      decoratedText += QString("color:%1;").arg(properties.value(key).value<QColor>().name());
    } else if (key == "fontStyle") {
      decoratedText += QString("font-style:%1;").arg(value);
    } else if (key == "fontWeight") {
      decoratedText += QString("font-weight:%1;").arg(value);
    } else if (key == "textDecorationLine") {
      decoratedText += QString("text-decoration:%1;").arg(value);
    }
  }
  decoratedText += "\">" + text.toHtmlEscaped() + "</span>";
  return decoratedText;
}
