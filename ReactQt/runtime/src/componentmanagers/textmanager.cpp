
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

#include "attachedproperties.h"
#include "bridge.h"
#include "layout/flexbox.h"
#include "propertyhandler.h"
#include "textmanager.h"
#include "utilities.h"
#include <cmath>

TextManager::TextManager(QObject* parent) : RawTextManager(parent) {}

TextManager::~TextManager() {}

ViewManager* TextManager::viewManager() {
    return this;
}

PropertyHandler* TextManager::propertyHandler(QObject* object) {
    Q_ASSERT(qobject_cast<QQuickItem*>(object) != nullptr);

    // we keep track of all assigned properties because if property not assigned explicitly,
    // it should be taken from parent
    return new PropertyHandler(object, [&](QObject* object, const QString& propertyName, const QVariant&) {
        m_explicitlySetProps[object].insert(propertyName);
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

QString TextManager::escape(const QString& text) {
    return text.toHtmlEscaped();
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
