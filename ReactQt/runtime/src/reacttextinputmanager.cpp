
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

#include "reactattachedproperties.h"
#include "reactbridge.h"
#include "reactevents.h"
#include "reactpropertyhandler.h"
#include "reacttextinputmanager.h"

const QString EVENT_ON_TEXT_CHANGE = "onChange";
const QString EVENT_ON_SELECTION_CHANGE = "onSelectionChange";

class ReactTextInputManagerPrivate {};

ReactTextInputManager::ReactTextInputManager(QObject* parent)
    : ReactViewManager(parent), d_ptr(new ReactTextInputManagerPrivate) {}

ReactTextInputManager::~ReactTextInputManager() {}

ReactViewManager* ReactTextInputManager::viewManager() {
    return this;
}

QString ReactTextInputManager::moduleName() {
    return "RCTTextInputViewManager";
}

QString ReactTextInputManager::qmlComponentFile() const {
    return ":/qml/ReactTextInput.qml";
}

void ReactTextInputManager::configureView(QQuickItem* view) const {
    ReactViewManager::configureView(view);
    view->setProperty("textInputManager", QVariant::fromValue((QObject*)this));
}

QStringList ReactTextInputManager::customDirectEventTypes() {
    return QStringList{
        EVENT_ON_TEXT_CHANGE, EVENT_ON_SELECTION_CHANGE,
    };
}

void ReactTextInputManager::sendTextEditedToJs(QQuickItem* textInput) {
    if (!textInput)
        return;

    QString text = textInput->property("text").toString();
    int reactTag = ReactAttachedProperties::get(textInput)->tag();
    QVariantList args = QVariantList{
        reactTag, normalizeInputEventName(EVENT_ON_TEXT_CHANGE), QVariantMap{{"target", reactTag}, {"text", text}}};

    bridge()->enqueueJSCall("RCTEventEmitter", "receiveEvent", args);
}

void ReactTextInputManager::sendSelectionChangeToJs(QQuickItem* textInput) {
    if (!textInput)
        return;
    int start = textInput->property("selectionStart").toInt();
    int end = textInput->property("selectionEnd").toInt();

    int reactTag = ReactAttachedProperties::get(textInput)->tag();
    QVariantMap startEnd = QVariantMap{{"start", start}, {"end", end}};

    QVariantList args = QVariantList{reactTag,
                                     normalizeInputEventName(EVENT_ON_SELECTION_CHANGE),
                                     QVariantMap{{"target", reactTag}, {"selection", startEnd}}};

    bridge()->enqueueJSCall("RCTEventEmitter", "receiveEvent", args);
}

#include "reacttextinputmanager.moc"
