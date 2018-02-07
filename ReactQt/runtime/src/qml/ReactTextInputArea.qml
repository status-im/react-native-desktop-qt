import QtQuick 2.7
import QtQuick.Controls 2.2
import React 0.1 as React
import  "../js/utils.js" as Utils


TextArea {
    id: textField

    property var textInputRoot: parent

    anchors.fill: textInputRoot

    text: textInputRoot.p_text
    color: textInputRoot.p_color
    placeholderText: textInputRoot.p_placeholderText
    selectionColor: textInputRoot.p_selectionColor
    objectName: textInputRoot.p_testID
    horizontalAlignment: Utils.alignmentQMLValue(textInputRoot.p_textAlign)

    selectByKeyboard: true
    selectByMouse: true

    background: Rectangle {
        border.color: textInputRoot.p_borderColor
        border.width: textInputRoot.p_borderWidth
        radius: textInputRoot.p_borderRadius
    }

    onTextChanged: textInputRoot.textInputManager.sendTextEditedToJs(textField)
    onSelectionStartChanged: textInputRoot.textInputManager.sendSelectionChangeToJs(textField)
    onSelectionEndChanged: textInputRoot.textInputManager.sendSelectionChangeToJs(textField)
    Keys.onPressed: textInputManager.sendOnKeyPressToJs(textField, event.text)
    onContentSizeChanged: textInputManager.sendOnContentSizeChange(textField, contentWidth, contentHeight)
    onEditingFinished: {
        textInputManager.sendOnSubmitEditingToJs(textField)
        textInputManager.sendOnEndEditingToJs(textField)
    }
    onFocusChanged: {
        if (focus) {
            textInputManager.sendOnFocusToJs(textField)
        }
    }

}

