
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

#include <QString>
#include <QVariant>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQmlProperty>

#include <QDebug>

#include "reacttextmanager.h"
#include "reactbridge.h"
#include "reactflexlayout.h"
#include "qmlpropertyhandler.h"

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
const char* k_writing_direction = "writingDirection";
const char* k_number_of_lines = "numberOfLines";
}



ReactTextManager::ReactTextManager(QObject* parent)
  : ReactRawTextManager(parent)
{
}

ReactTextManager::~ReactTextManager()
{
}

ReactViewManager* ReactTextManager::viewManager()
{
  return this;
}

ReactPropertyHandler* ReactTextManager::propertyHandler(QObject* object)
{
  Q_ASSERT(qobject_cast<QQuickItem*>(object) != nullptr);
  return new QmlPropertyHandler(object);
}

QString ReactTextManager::moduleName()
{
  return "RCTTextManager";
}

QList<ReactModuleMethod*> ReactTextManager::methodsToExport()
{
  return QList<ReactModuleMethod*>{};
}

QVariantMap ReactTextManager::constantsToExport()
{
  return QVariantMap{};
}

bool ReactTextManager::shouldLayout() const
{
  return true;
}

QString ReactTextManager::qmlComponentFile() const
{
  return ":/qml/ReactText.qml";
}

void ReactTextManager::configureView(QQuickItem* view) const
{
  ReactRawTextManager::configureView(view);
  view->setProperty("textManager", QVariant::fromValue((QObject*)this));
}


void ReactTextManager::hookLayout(QQuickItem* textItem)
{
  ReactFlexLayout* fl = ReactFlexLayout::get(textItem);
  fl->setMeasureFunction([=](double width, FlexMeasureMode widthMode, double height, FlexMeasureMode heightMode) {

    QString ts = ReactTextManager::textWithProperties(textItem, PropertyMap());
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


QString ReactTextManager::convertPropsToHtml(const QString& text, const QVariantMap& properties)
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


QString ReactTextManager::textWithProperties(QQuickItem* item, const PropertyMap& properties) {
  PropertyMap mp = ReactTextManager::props(item);
  mp.insert(properties.begin(), properties.end());

  QString text;
  for (auto& childItem : item->childItems())
  {
    QString childType = childItem->property("typeName").toString();
    if (childType == "ReactRawText")
    {
      QString childText = childItem->property("p_text").toString();
      text += ReactTextManager::convertPropsToHtml(childText, QMap<QString, QVariant>(mp));
    }
    else if (childType == "ReactText")
    {
      text += ReactTextManager::textWithProperties(childItem,mp);
    }
  }
  return text;
}

ReactTextManager::PropertyMap ReactTextManager::props(QQuickItem* item)
{
  PropertyMap props;
  QString qmlPropertyname;

  qmlPropertyname = QString("p_"+QString(k_allow_font_scaling));
  props[k_allow_font_scaling] = item->property(qmlPropertyname.toStdString().c_str());

  qmlPropertyname = QString("p_"+QString(k_font_family));
  props[k_font_family] = item->property(qmlPropertyname.toStdString().c_str());

  qmlPropertyname = QString("p_"+QString(k_font_size));
  props[k_font_size] = item->property(qmlPropertyname.toStdString().c_str());

  qmlPropertyname = QString("p_"+QString(k_highlighted));
  props[k_highlighted] = item->property(qmlPropertyname.toStdString().c_str());

  qmlPropertyname = QString("p_"+QString(k_color));
  props[k_color] = item->property(qmlPropertyname.toStdString().c_str());

  qmlPropertyname = QString("p_"+QString(k_font_style));
  props[k_font_style] = item->property(qmlPropertyname.toStdString().c_str());

  qmlPropertyname = QString("p_"+QString(k_font_weight));
  props[k_font_weight] = item->property(qmlPropertyname.toStdString().c_str());

  qmlPropertyname = QString("p_"+QString(k_letter_spacing));
  props[k_letter_spacing] = item->property(qmlPropertyname.toStdString().c_str());

  qmlPropertyname = QString("p_"+QString(k_line_height));
  props[k_line_height] = item->property(qmlPropertyname.toStdString().c_str());

  qmlPropertyname = QString("p_"+QString(k_text_align));
  props[k_text_align] = item->property(qmlPropertyname.toStdString().c_str());

  qmlPropertyname = QString("p_"+QString(k_text_decoration_line));
  props[k_text_decoration_line] = item->property(qmlPropertyname.toStdString().c_str());

  qmlPropertyname = QString("p_"+QString(k_text_decoration_style));
  props[k_text_decoration_style] = item->property(qmlPropertyname.toStdString().c_str());

  qmlPropertyname = QString("p_"+QString(k_text_decoration_color));
  props[k_text_decoration_color] = item->property(qmlPropertyname.toStdString().c_str());

  qmlPropertyname = QString("p_"+QString(k_writing_direction));
  props[k_writing_direction] = item->property(qmlPropertyname.toStdString().c_str());

  qmlPropertyname = QString("p_"+QString(k_number_of_lines));
  props[k_number_of_lines] = item->property(qmlPropertyname.toStdString().c_str());

  return props;

}
