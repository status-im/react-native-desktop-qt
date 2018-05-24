import QtQuick 2.7
import QtQuick.Controls 2.2
import React 0.1 as React
import  "../js/utils.js" as Utils


TextField {
    id: textField

    property var textInputRoot: parent

    anchors.fill: textInputRoot
    text: textInputRoot.p_text
    color: textInputRoot.p_color
    placeholderText: textInputRoot.p_placeholderText
    selectionColor: textInputRoot.p_selectionColor
    objectName: textInputRoot.p_nativeID
    horizontalAlignment: Utils.alignmentQMLValue(textInputRoot.p_textAlign)
    echoMode: textInputRoot.p_secureTextEntry ? TextInput.Password : TextInput.Normal

    selectByMouse: true
    background: Rectangle {
        border.color: textInputRoot.p_borderColor
        border.width: textInputRoot.p_borderWidth
        radius: textInputRoot.p_borderRadius
    }

    onTextChanged:              textInputManager.sendTextEditedToJs(textField)
    onCursorPositionChanged:    textInputManager.sendSelectionChangeToJs(textField)
    onAccepted:                 textInputManager.sendOnSubmitEditingToJs(textField)
    onEditingFinished:          textInputManager.sendOnEndEditingToJs(textField)
    onContentSizeChanged:       {
        if(textInputManager)
            textInputManager.sendOnContentSizeChange(textField, contentWidth, contentHeight)
    }
    Keys.onPressed: textInputManager.sendOnKeyPressToJs(textField,
                                                        textInputRoot.keyText(event.key, event.text),
                                                        textInputRoot.keyModifiers(event.modifiers))

    onFocusChanged: {
        if (focus) {
            textInputManager.sendOnFocusToJs(textField)
        }
    }


}
