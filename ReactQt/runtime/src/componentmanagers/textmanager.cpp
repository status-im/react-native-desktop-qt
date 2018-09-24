
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

#include <QQmlComponent>
#include <QQmlProperty>
#include <QQuickItem>
#include <QString>
#include <QVariant>

#include <QDebug>

#include "bridge.h"
#include "layout/flexbox.h"
#include "propertyhandler.h"
#include "textmanager.h"
#include <QRegExp>
#include <cmath>

const char FLEXBOX[] = "flexbox";

TextManager::TextManager(QObject* parent) : RawTextManager(parent) {}

TextManager::~TextManager() {}

ViewManager* TextManager::viewManager() {
    return this;
}

PropertyHandler* TextManager::propertyHandler(QObject* object) {
    Q_ASSERT(qobject_cast<QQuickItem*>(object) != nullptr);

    // we keep track of all assigned properties because if property not assigned explicitly,
    // it should be taken from parent
    return new PropertyHandler(object, [&](QObject* object, QMetaProperty property, const QVariant& value) {
        m_explicitlySetProps[object].insert(property.name());
    });
}

QString TextManager::moduleName() {
    return "RCTTextManager";
}

bool TextManager::shouldLayout() const {
    return true;
}

QString TextManager::qmlComponentFile(const QVariantMap& properties) const {
    return "qrc:/qml/ReactText.qml";
}

void TextManager::configureView(QQuickItem* view) const {
    ViewManager::configureView(view);
    view->setProperty("textManager", QVariant::fromValue((QObject*)this));
}

void TextManager::updateMeasureFunction(QQuickItem* textItem) {

    Flexbox* flexbox = Flexbox::findFlexbox(textItem);
    if (!flexbox) {
        return;
    }

    bool childIsTopReactTextInTextHierarchy = textItem->property("textIsTopInBlock").toBool();

    if (childIsTopReactTextInTextHierarchy) {
        flexbox->setMeasureFunction([=](
            YGNodeRef /*node*/, float width, YGMeasureMode /*widthMode*/, float height, YGMeasureMode /*heightMode*/) {
            resizeToWidth(textItem, width);
            resizeToHeight(textItem, height);
            float h = textItem->height();
            float w = textItem->width();
            return YGSize{w, h};
        });
    } else {
        flexbox->setMeasureFunction(nullptr);
    }
}

void TextManager::manageFlexbox(QQuickItem* textItem, bool textIsTopInBlock) {
    // Only topmost text item in a set of nested ones can have a flexbox node.
    if (textIsTopInBlock) {

        Flexbox* f = new Flexbox(textItem);
        f->setControl(textItem);
        f->setViewManager(this);
        textItem->setProperty(FLEXBOX, QVariant::fromValue(f));
    } else {
        QVariant flexboxVar = textItem->property(FLEXBOX);
        if (flexboxVar.canConvert<Flexbox*>()) {
            Flexbox* f = flexboxVar.value<Flexbox*>();
            if (f) {
                f->deleteLater();
                textItem->setProperty(FLEXBOX, QVariant::fromValue(nullptr));
            }
        }
    }
}

QString TextManager::textToHtml(QQuickItem* textItem, const QString& text) {
    // get props values, own or nested
    QString fontFamily = nestedPropertyValue(textItem, "p_fontFamily").toString();
    QString color = nestedPropertyValue(textItem, "p_color").toString();
    QString backgroundColor = nestedPropertyValue(textItem, "p_backgroundColor").toString();
    QString fontWeight = nestedPropertyValue(textItem, "p_fontWeight").toString();
    QString fontSize = nestedPropertyValue(textItem, "p_fontSize").toString();
    QString fontStyle = nestedPropertyValue(textItem, "p_fontStyle").toString();
    QString textDecorLine = nestedPropertyValue(textItem, "p_textDecorationLine").toString();

    QString result =
        "<span style=\"white-space: pre; " + (!fontFamily.isEmpty() ? ("font-family:" + fontFamily + ";") : "") +
        (!fontSize.isEmpty() ? ("font-size:" + fontSize + "pt;") : "") +
        (!color.isEmpty() ? ("color:" + color + ";") : "") +
        (!(backgroundColor == "#00000000") ? ("background-color:" + backgroundColor + ";") : "") +
        (!fontStyle.isEmpty() ? ("font-style:" + fontStyle + ";") : "") +
        (!fontWeight.isEmpty() ? ("font-weight:" + fontWeight + ";") : "") +
        (!textDecorLine.isEmpty() ? ("text-decoration:" + textDecorLine + ";") : "") + "\">" + text + "</span>";

    return result.replace(QRegExp("/\n / g"), "<br>");
}

void TextManager::resizeToWidth(QQuickItem* textItem, double width) {
    textItem->setWidth(width);
    double contentWidth = textItem->property("contentWidth").value<double>();
    double sw = 0;
    if (std::isnan(width)) {
        sw = contentWidth;
    } else {
        sw = contentWidth == 0 ? width : qMin(contentWidth, width);
    }
    textItem->setWidth(sw);
}

void TextManager::resizeToHeight(QQuickItem* textItem, double height) {
    textItem->setHeight(height);
    double contentHeight = textItem->property("contentHeight").value<double>();
    double sh = 0;
    if (std::isnan(height)) {
        sh = contentHeight;
    } else {
        sh = contentHeight == 0 ? height : qMin(contentHeight, height);
    }
    textItem->setHeight(sh);
}

QVariant TextManager::nestedPropertyValue(QQuickItem* item, const QString& propertyName) {
    // return value if it is explicitly set on current item
    if (propertyExplicitlySet(item, propertyName)) {
        return item->property(propertyName.toStdString().c_str());
    }

    // check if any of text parents has this property explicitly set
    QQuickItem* parent = parentTextItem(item);
    while (parent) {
        if (propertyExplicitlySet(parent, propertyName)) {
            return parent->property(propertyName.toStdString().c_str());
        }
        parent = parentTextItem(parent);
    }

    // no parents has this property explicitly set, so we use default value
    return item->property(propertyName.toStdString().c_str());
}

QQuickItem* TextManager::parentTextItem(QQuickItem* textItem) {
    if (!textItem)
        return nullptr;

    auto visualParent = textItem->parentItem();
    if (!visualParent)
        return nullptr;

    QVariant typeName = visualParent->property("typeName");
    if (!typeName.isValid())
        return nullptr;

    return (typeName.toString() == "ReactText") ? static_cast<QQuickItem*>(visualParent) : nullptr;
}

bool TextManager::propertyExplicitlySet(QQuickItem* item, const QString& propertyName) {
    if (m_explicitlySetProps.contains(item)) {
        return m_explicitlySetProps[item].contains(propertyName);
    }
    return false;
}
