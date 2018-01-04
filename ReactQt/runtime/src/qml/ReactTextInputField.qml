import QtQuick 2.7
import QtQuick.Controls 2.2
import React 0.1 as React
import  "../js/utils.js" as Utils


TextField {
    id: textField
    anchors.fill: parent

    text: parent.p_text
    color: parent.p_color
    placeholderText: parent.p_placeholderText
    selectionColor: parent.p_selectionColor
    objectName: parent.p_testID
    horizontalAlignment: Utils.alignmentQMLValue(parent.p_textAlign)
    echoMode: parent.p_secureTextEntry ? TextInput.Password : TextInput.Normal

    selectByMouse: true
    background: Rectangle {
        border.color:  "gray"
    }

    onTextChanged: textInputManager.sendTextEditedToJs(textField)

    onSelectionStartChanged: {
        textInputManager.sendSelectionChangeToJs(textField)
    }
    onSelectionEndChanged: {
        textInputManager.sendSelectionChangeToJs(textField)
    }
    onAccepted: {
        textInputManager.sendOnSubmitEditingToJs(textField)
    }
    onEditingFinished: {
        textInputManager.sendOnEndEditingToJs(textField)
    }
    onFocusChanged: {
        if (focus) {
            textInputManager.sendOnFocusToJs(textField)
        }
    }
    Keys.onPressed: {
        textInputManager.sendOnKeyPressToJs(textField, event.text)
    }

}
