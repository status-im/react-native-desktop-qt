import QtQuick 2.7
import QtQuick.Controls 2.2
import React 0.1 as React
import  "../js/utils.js" as Utils


Flickable {
    id: textField
    property var textInputRoot: parent
    property alias cursorPosition: textArea.cursorPosition
    property alias text: textArea.text
    property alias textAreaLength: textArea.length
    anchors.fill: textInputRoot

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
            if(!textInputRoot.jsTextChange) {
                textInputRoot.textInputManager.sendTextEditedToJs(textField)
            }
        }
        onCursorPositionChanged: textInputRoot.textInputManager.sendSelectionChangeToJs(textField)
        Keys.onPressed: {
            var keyText = textInputRoot.keyText(event.key, event.text);
            var modifiers = textInputRoot.keyModifiers(event.modifiers);
            event.accepted = textInputManager.onKeyPressed(textField,
                                          keyText,
                                          modifiers,
                                          textInputRoot.p_submitShortcut.key,
                                          textInputRoot.p_submitShortcut.modifiers)
        }
        onContentSizeChanged: {            
            if(textInputManager)
                textInputManager.sendOnContentSizeChange(textField, contentWidth, contentHeight)

            var fl = textInputRoot.flexbox;

            if(fl && textInputRoot.heightSetInternally){
                textInputRoot.flexbox.p_height = contentHeight;
                if(textInputRoot.textInputManager) {
                    textInputRoot.textInputManager.requestRootPolish();
                }
            }

        }
        onEditingFinished: {
            textInputManager.sendOnEndEditingToJs(textField)
        }

        onFocusChanged: {
            if (focus) {
                textInputManager.sendOnFocusToJs(textField)
            }
        }
    }
}

