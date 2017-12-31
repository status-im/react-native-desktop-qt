
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
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
#include "propertyhandler.h"
#include "textinputmanager.h"
#include "utilities.h"

const QString EVENT_ON_TEXT_CHANGE = "onChange";
const QString EVENT_ON_SELECTION_CHANGE = "onSelectionChange";

class TextInputManagerPrivate {};

TextInputManager::TextInputManager(QObject* parent) : ViewManager(parent), d_ptr(new TextInputManagerPrivate) {}

TextInputManager::~TextInputManager() {}

ViewManager* TextInputManager::viewManager() {
    return this;
}

QString TextInputManager::moduleName() {
    return "RCTTextInputViewManager";
}

QString TextInputManager::qmlComponentFile() const {
    return "qrc:/qml/ReactTextInput.qml";
}

void TextInputManager::configureView(QQuickItem* view) const {
    ViewManager::configureView(view);
    view->setProperty("textInputManager", QVariant::fromValue((QObject*)this));
}

QStringList TextInputManager::customDirectEventTypes() {
    return QStringList{
        EVENT_ON_SELECTION_CHANGE,
    };
}

void TextInputManager::sendTextEditedToJs(QQuickItem* textInput) {
    if (!textInput)
        return;

    QString text = textInput->property("text").toString();
    int parentTag = tag(textInput->parentItem());

    notifyJsAboutEvent(parentTag, EVENT_ON_TEXT_CHANGE, QVariantMap{{"target", parentTag}, {"text", text}});
}

void TextInputManager::sendSelectionChangeToJs(QQuickItem* textInput) {
    if (!textInput)
        return;
    int start = textInput->property("selectionStart").toInt();
    int end = textInput->property("selectionEnd").toInt();

    QVariantMap startEnd = QVariantMap{{"start", start}, {"end", end}};

    int parentTag = tag(textInput->parentItem());

    notifyJsAboutEvent(
        parentTag, EVENT_ON_SELECTION_CHANGE, QVariantMap{{"target", parentTag}, {"selection", startEnd}});
}

#include "textinputmanager.moc"
