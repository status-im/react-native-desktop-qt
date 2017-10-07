import QtQuick 2.7
import QtQuick.Controls 2.2
import React 0.1 as React
import  "../js/utils.js" as Utils

TextArea {
    id: textInputRoot
    property var textInputManager: null

    property string p_text
    property color p_color
    property bool p_multiline: false
    property bool p_onChange: false
    property bool p_onSelectionChange: false
    property string p_placeholderText
    property string p_textAlign: "left"
    property string p_selectionColor: "lightblue"
    property string p_testID


    property var flexbox: React.Flexbox {control: textInputRoot}

    text: p_text
    color: p_color
    placeholderText: p_placeholderText
    selectionColor: p_selectionColor
    objectName: p_testID
    horizontalAlignment: Utils.alignmentQMLValue(p_textAlign)

    selectByKeyboard: true
    selectByMouse: true
    background: Rectangle {
        border.color:  "gray"
    }

    onTextChanged: {
        if (!p_multiline && textInputRoot.text.indexOf('\n') != -1) {
            //TODO. should not trigger textChanged
            textInputRoot.text = textInputRoot.text.replace('\n', '');
            return;
        }
        textInputManager.sendTextEditedToJs(textInputRoot)
    }
    onSelectionStartChanged: {
        textInputManager.sendSelectionChangeToJs(textInputRoot)
    }
    onSelectionEndChanged: {
        textInputManager.sendSelectionChangeToJs(textInputRoot)
    }
}
