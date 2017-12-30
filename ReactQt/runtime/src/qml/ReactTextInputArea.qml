import QtQuick 2.7
import QtQuick.Controls 2.2
import React 0.1 as React
import  "../js/utils.js" as Utils


TextArea {
    id: textField
    anchors.fill: parent

    text: parent.p_text
    color: parent.p_color
    placeholderText: parent.p_placeholderText
    selectionColor: parent.p_selectionColor
    objectName: parent.p_testID
    horizontalAlignment: Utils.alignmentQMLValue(parent.p_textAlign)

    selectByKeyboard: true
    selectByMouse: true
    background: Rectangle {
        border.color:  "gray"
    }

    onTextChanged: parent.textInputManager.sendTextEditedToJs(textField)

    onSelectionStartChanged: {
        parent.textInputManager.sendSelectionChangeToJs(textField)
    }
    onSelectionEndChanged: {
        parent.textInputManager.sendSelectionChangeToJs(textField)
    }
}

