import QtQuick 2.7
import QtQuick.Controls 2.2
import React 0.1 as React
import  "../js/utils.js" as Utils


TextField {
    id: textField

    property var textInputRoot: parent

    anchors.fill: textInputRoot
    color: textInputRoot.p_color
    placeholderText: textInputRoot.p_placeholderText
    selectionColor: textInputRoot.p_selectionColor
    objectName: textInputRoot.p_nativeID
    horizontalAlignment: Utils.alignmentQMLValue(textInputRoot.p_textAlign)
    echoMode: textInputRoot.p_secureTextEntry ? TextInput.Password : TextInput.Normal
    font.pointSize: textInputRoot.p_fontSize
    font.family: textInputRoot.p_fontFamily
    font.weight: textInputRoot.p_fontWeightEnum
    padding: 0
    focus: textInputRoot.p_autoFocus


    selectByMouse: true
    background: Rectangle {
        color: textInputRoot.p_backgroundColor
        border.color: textInputRoot.p_borderColor
        border.width: textInputRoot.p_borderWidth
        radius: textInputRoot.p_borderRadius
    }

    onTextChanged: {
        if(!textInputRoot.jsTextChange) {
            textInputRoot.textInputManager.sendTextEditedToJs(textField)
        }
    }
    onCursorPositionChanged:    textInputManager.sendSelectionChangeToJs(textField)
    onEditingFinished:          textInputManager.sendOnEndEditingToJs(textField)
    onContentSizeChanged:       {
        if(textInputManager)
            textInputManager.sendOnContentSizeChange(textField, contentWidth, contentHeight)
    }
    Keys.onPressed: {

        var keyText = textInputRoot.keyText(event.key, event.text);
        var modifiers = textInputRoot.keyModifiers(event.modifiers);
        event.accepted = textInputManager.onKeyPressed(textField,
                                      keyText,
                                      modifiers,
                                      textInputRoot.p_submitShortcut.key,
                                      textInputRoot.p_submitShortcut.modifiers)
    }

    onFocusChanged: {
        if (focus) {
            textInputManager.sendOnFocusToJs(textField)
        }
    }


}
