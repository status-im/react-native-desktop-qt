
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


QString ReactTextProperties::textAlign() const {
  QQmlProperty p(item, "horizontalAlignment");
  switch (p.read().toInt()) {
  case Qt::AlignLeft: return "left";
  case Qt::AlignRight: return "right";
  case Qt::AlignHCenter: return "center";
  case Qt::AlignJustify: return "justify";
  }
}

void ReactTextProperties::setTextAlign(const QString& textAlign) {
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

void ReactTextProperties::setDirty(bool dirty) {
  this->dirty = dirty;
  ReactTextProperties* tp = ReactTextProperties::get(item->parentItem(), false);
  if (tp != nullptr) {
    tp->setDirty(dirty);
  }
  ReactFlexLayout::get(item)->setDirty(dirty);
}

QString ReactTextProperties::textWithProperties(const property_map& properties) {
  property_map mp = this->properties;
  mp.insert(properties.begin(), properties.end());

  getItemProperties();

  QString text;
  for (auto& childItem : item->childItems())
  {
    QString childType = childItem->property("typeName").toString();
    if (childType == "ReactRawText")
    {
      QString childText = childItem->property("p_text").toString();
      //text += ReactTextProperties::convertPropsToHtml(tp->text(), QMap<QString, QVariant>(mp));
      qDebug()<<"childText: "<<childText;
      text += ReactTextProperties::convertPropsToHtml(childText, QMap<QString, QVariant>(mp));
    }
    else if (childType == "ReactText")
    {
      auto textProps = ReactTextProperties::get(childItem);
      text += textProps->textWithProperties(mp);
    }
  }
  return text;
}

QVariantMap ReactTextProperties::getItemProperties()
{
  //QVariantMap returnedValue;
  QJSValue returnedValue;
  qDebug()<<QMetaObject::invokeMethod(item, "bzz", Q_RETURN_ARG(QJSValue, returnedValue));
  qDebug() << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
  qDebug()<<"typeName: "<<item->property("typeName").toString();
  qDebug() << "QML function returned:" << returnedValue.toString();

//  return returnedValue;
  return QVariantMap();
}



ReactTextProperties::ReactTextProperties(QObject* parent)
  : ReactPropertyHandler(parent)
{
  dirty = false;
  item = qobject_cast<QQuickItem*>(parent);
  if (item == nullptr) {
    qCritical() << "ReactTextProperties only applies to visual items";
  }
}

ReactTextProperties::~ReactTextProperties()
{
}

bool ReactTextProperties::allowFontScaling() const
{
  return value<bool>(k_allow_font_scaling);
}

void ReactTextProperties::setAllowFontScaling(bool allowFontScaling)
{
  item->setProperty("fontSizeMode", allowFontScaling ? 3 : 0);// XXX:
  setDirty(true);
}

QString ReactTextProperties::fontFamily() const
{
  return value<QString>(k_font_family);
}

void ReactTextProperties::setFontFamily(const QString& fontFamily)
{
  setValue(k_font_family, fontFamily);
}

double ReactTextProperties::fontSize() const
{
  return value<double>(k_font_size, -1);
}

void ReactTextProperties::setFontSize(double fontSize)
{
  setValue(k_font_size, fontSize);
}

bool ReactTextProperties::isHighlighted() const
{
  return value<bool>(k_highlighted);
}

void ReactTextProperties::setHighlighted(bool isHighlighted)
{
  setValue(k_highlighted, isHighlighted);
}

QColor ReactTextProperties::color() const
{
  return value<QColor>(k_color);
}

void ReactTextProperties::setColor(const QColor& color)
{
  setValue(k_color, color);
}

QString ReactTextProperties::fontStyle() const
{
  return value<QString>(k_font_style);
}

void ReactTextProperties::setFontStyle(const QString& fontStyle)
{
  setValue(k_font_style, fontStyle);
}

QString ReactTextProperties::fontWeight()
{
  return value<QString>(k_font_weight);
}

void ReactTextProperties::setFontWeight(const QString& fontWeight)
{
  setValue(k_font_weight, fontWeight);
}

double ReactTextProperties::letterSpacing()
{
  return value<double>(k_letter_spacing);
}

void ReactTextProperties::setLetterSpacing(double letterSpacing)
{
  setValue(k_letter_spacing, letterSpacing);
}

double ReactTextProperties::lineHeight() const
{
  return value<double>(k_line_height, -1);
}

void ReactTextProperties::setLineHeight(double lineHeight)
{
  setValue(k_line_height, lineHeight);
}

QString ReactTextProperties::textDecorationLine() const
{
  return value<QString>(k_text_decoration_line);
}

void ReactTextProperties::setTextDecorationLine(const QString& textDecorationLine)
{
  setValue(k_text_decoration_line, textDecorationLine);
}

QString ReactTextProperties::textDecorationStyle() const
{
  return value<QString>(k_text_decoration_style);
}

void ReactTextProperties::setTextDecorationStyle(const QString& textDecorationStyle)
{
  setValue(k_text_decoration_style, textDecorationStyle);
}

QColor ReactTextProperties::textDecorationColor() const
{
  return value<QString>(k_text_decoration_color);
}

void ReactTextProperties::setTextDecorationColor(const QColor& textDecorationColor)
{
  setValue(k_text_decoration_style, textDecorationColor);
}

QString ReactTextProperties::writingDirection() const
{
  return value<QString>(k_writing_direction);
}

void ReactTextProperties::setWritingDirection(const QString& writingDirection)
{
  setValue(k_writing_direction, writingDirection);
}

int ReactTextProperties::numberOfLines() const
{
  return value<int>(k_number_of_lines, -1);
}

void ReactTextProperties::setNumberOfLines(int numberOfLines)
{
  item->setProperty("maxiumLineCount", numberOfLines);
  setDirty(true);
}


void ReactTextProperties::hookLayout(QQuickItem* textItem)
{
  ReactFlexLayout* fl = ReactFlexLayout::get(textItem);
  fl->setMeasureFunction([=](double width, FlexMeasureMode widthMode, double height, FlexMeasureMode heightMode) {

    QString ts = textWithProperties(property_map());
    qDebug()<<"========== TEXT =============";
    qDebug()<<ts;
    textItem->setProperty("text", ts);

    double contentWidth = textItem->property("contentWidth").value<double>();
    double sw = 0;
    if (std::isnan(width)) {
      sw = contentWidth;
    } else {
      sw = contentWidth == 0 ? width : qMin(contentWidth, width);
    }

    textItem->setWidth(sw);

    return std::make_pair(float(sw), textItem->property("contentHeight").value<float>());
  });
}

ReactTextProperties* ReactTextProperties::get(QQuickItem* item, bool create)
{
  return qobject_cast<ReactTextProperties*>(qmlAttachedPropertiesObject<ReactTextProperties>(item, create));
}

ReactTextProperties* ReactTextProperties::qmlAttachedProperties(QObject* object)
{
  return new ReactTextProperties(object);
}


QString ReactTextProperties::convertPropsToHtml(const QString& text, const QVariantMap& properties)
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
