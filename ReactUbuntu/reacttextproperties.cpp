
#include <map>

#include <QQuickItem>

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

  void apply() {
    QString text = textWithProperties(item, property_map());
    item->setProperty("text", text);
    item->setProperty("visible", true);

    ReactFlexLayout* fl = ReactFlexLayout::get(item);
    fl->setWidth(item->property("contentWidth").value<double>());
    fl->setHeight(item->property("contentHeight").value<double>());
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
  }

  static ReactTextPropertiesPrivate* get(ReactTextProperties* rtp) {
    return rtp->d_func();
  }

private:
  QString textWithProperties(QQuickItem* item, const property_map& properties) {
    property_map mp = this->properties;
    mp.insert(properties.begin(), properties.end());

    QString text;
    for (auto c : item->childItems()) {
      ReactRawTextProperties* rtp = ReactRawTextProperties::get(c, false);
      if (rtp != nullptr) {
        text += rtp->textWithProperties(QMap<QString, QVariant>(mp));
      }
      text += textWithProperties(c, mp);
    }
    return text;
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
  // XXX:
  d_func()->item->setProperty("fontSizeMode", allowFontScaling ? 3 : 0);
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
  return d_func()->value<QString>(k_text_align);
}

void ReactTextProperties::setTextAlign(const QString& textAlign)
{
  d_func()->setValue(k_text_align, textAlign);
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
  d_func()->item->setProperty("maxiumLineCount", numberOfLines);
}

void ReactTextProperties::polish(QQuickItem* item)
{
  ReactTextProperties* rtp = ReactTextProperties::get(item, false);
  if (rtp != nullptr) {
    // apply text properties to all children
    ReactTextPropertiesPrivate::get(rtp)->apply();
  } else {
    for (auto c : item->childItems()) {
      polish(c);
    }
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
