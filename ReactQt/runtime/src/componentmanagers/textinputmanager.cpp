
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
#include <QVariantMap>

#include <QDebug>

#include "attachedproperties.h"
#include "bridge.h"
#include "propertyhandler.h"
#include "textinputmanager.h"
#include "utilities.h"

const QString EVENT_ON_TEXT_CHANGE = "onChange";
const QString EVENT_ON_SELECTION_CHANGE = "onSelectionChange";
const QString EVENT_ON_SUBMIT_EDITING = "onSubmitEditing";
const QString EVENT_ON_END_EDITING = "onEndEditing";
const QString EVENT_ON_FOCUS = "onFocus";
const QString EVENT_ON_BLUR = "onBlur";
const QString EVENT_ON_KEY_PRESS = "onKeyPress";
const QString EVENT_ON_CONTENT_SIZE_CHANGE = "onContentSizeChange";

class TextInputManagerPrivate {};

TextInputManager::TextInputManager(QObject* parent) : ViewManager(parent), d_ptr(new TextInputManagerPrivate) {}

TextInputManager::~TextInputManager() {}

ViewManager* TextInputManager::viewManager() {
    return this;
}

QString TextInputManager::moduleName() {
    return "RCTTextInputViewManager";
}

QString TextInputManager::qmlComponentFile(const QVariantMap& properties) const {
    return "qrc:/qml/ReactTextInput.qml";
}

void TextInputManager::configureView(QQuickItem* view) const {
    ViewManager::configureView(view);
    view->setProperty("textInputManager", QVariant::fromValue((QObject*)this));
}

QStringList TextInputManager::customDirectEventTypes() {
    return QStringList{EVENT_ON_SELECTION_CHANGE, EVENT_ON_KEY_PRESS, EVENT_ON_CONTENT_SIZE_CHANGE};
}

void TextInputManager::sendTextEditedToJs(QQuickItem* textInput) {
    QString text = textInput->property("text").toString();
    sendTextInputEvent(textInput, EVENT_ON_TEXT_CHANGE, QVariantMap{{"text", text}});
}

void TextInputManager::sendSelectionChangeToJs(QQuickItem* textInput) {
    if (!textInput)
        return;
    int start = textInput->property("selectionStart").toInt();
    int end = textInput->property("selectionEnd").toInt();

    // TODO: Generation of onSelectionChange event causes issue
    // https://github.com/status-im/react-native-desktop/issues/210
    // sendTextInputEvent(
    //    textInput, EVENT_ON_SELECTION_CHANGE, QVariantMap{{"selection", QVariantMap{{"start", start}, {"end",
    //    end}}}});
}

void TextInputManager::sendOnSubmitEditingToJs(QQuickItem* textInput) {
    sendTextInputEvent(textInput, EVENT_ON_SUBMIT_EDITING);
}

void TextInputManager::sendOnEndEditingToJs(QQuickItem* textInput) {
    sendTextInputEvent(textInput, EVENT_ON_END_EDITING);
    sendTextInputEvent(textInput, EVENT_ON_BLUR);
}

void TextInputManager::sendOnFocusToJs(QQuickItem* textInput) {
    sendTextInputEvent(textInput, EVENT_ON_FOCUS);
}

void TextInputManager::sendOnKeyPressToJs(QQuickItem* textInput, QString keyText, QVariantList modifiers) {
    sendTextInputEvent(textInput, EVENT_ON_KEY_PRESS, QVariantMap{{"key", keyText}, {"modifiers", modifiers}});
}

void TextInputManager::sendOnContentSizeChange(QQuickItem* textInput, double width, double height) {
    sendTextInputEvent(textInput,
                       EVENT_ON_CONTENT_SIZE_CHANGE,
                       QVariantMap{{"contentSize", QVariantMap{{"width", width}, {"height", height}}}});
}

bool TextInputManager::onKeyPressed(QQuickItem* textInput,
                                    QString keyText,
                                    QVariantList modifiers,
                                    QString submitKeyText,
                                    QVariantList submitModifiers) {
    if (!submitKeyText.isEmpty() && (keyText == submitKeyText) && (modifiers == submitModifiers)) {
        sendOnSubmitEditingToJs(textInput);
        return true;
    } else {
        sendOnKeyPressToJs(textInput, keyText, modifiers);
        return false;
    }
}

void TextInputManager::sendTextInputEvent(QQuickItem* textInput, QString eventName, QVariantMap additionalEventData) {
    if (!textInput)
        return;

    QString text = textInput->property("text").toString();
    int parentTag = tag(textInput->parentItem());

    QVariantMap eventData = QVariantMap{{"target", parentTag}};
    if (!additionalEventData.isEmpty()) {
        for (auto iterator = additionalEventData.constBegin(); iterator != additionalEventData.constEnd(); ++iterator) {
            eventData.insert(iterator.key(), iterator.value());
        }
    }
    notifyJsAboutEvent(parentTag, eventName, eventData);
}

#include "textinputmanager.moc"
