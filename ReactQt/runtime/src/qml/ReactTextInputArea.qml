import QtQuick 2.7
import QtQuick.Controls 2.2
import React 0.1 as React
import  "../js/utils.js" as Utils


Flickable {
    id: textField
    property var textInputRoot: parent
    property alias cursorPosition: textArea.cursorPosition
    property alias text: textArea.text
    anchors.fill: textInputRoot
    ScrollBar.vertical: ScrollBar {}

    TextArea.flickable: TextArea {

        id: textArea
        color: textInputRoot.p_color
        placeholderText: textInputRoot.p_placeholderText
        selectionColor: textInputRoot.p_selectionColor
        objectName: textInputRoot.p_nativeID
        horizontalAlignment: Utils.alignmentQMLValue(textInputRoot.p_textAlign)
        wrapMode: TextEdit.Wrap
        font.pointSize: textInputRoot.p_fontSize
        font.family: textInputRoot.p_fontFamily
        font.weight: textInputRoot.p_fontWeightEnum
        padding: 0
        focus: textInputRoot.p_autoFocus

        selectByKeyboard: true
        selectByMouse: true

        background: Rectangle {
            color: textInputRoot.p_backgroundColor
            border.color: textInputRoot.p_borderColor
            border.width: textInputRoot.p_borderWidth
            radius: textInputRoot.p_borderRadius
        }

        onTextChanged: {
            if(textInputRoot.sendTextChanged) {
                textInputRoot.textInputManager.sendTextEditedToJs(textField)
            }
        }
        onCursorPositionChanged: textInputRoot.textInputManager.sendSelectionChangeToJs(textField)
        Keys.onPressed: textInputManager.sendOnKeyPressToJs(textField,
                                                            textInputRoot.keyText(event.key, event.text),
                                                            textInputRoot.keyModifiers(event.modifiers))
        onContentSizeChanged: {
            if(textInputManager)
                textInputManager.sendOnContentSizeChange(textField, contentWidth, contentHeight)
        }
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
}

