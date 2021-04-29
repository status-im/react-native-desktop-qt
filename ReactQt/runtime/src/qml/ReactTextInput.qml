  import QtQuick 2.7
import QtQuick.Controls 2.2
import React 0.1 as React
import  "../js/utils.js" as Utils

Item {
    id: textInputRoot
    property var textInputManager: null
    property var textInputControl: null

    property string p_text : textInputControl ? textInputControl.text : ""
    property color p_color
    property bool p_multiline: false
    property bool p_onChange: false
    property bool p_onSelectionChange: false
    property string p_placeholderText
    property string p_textAlign: "left"
    property string p_selectionColor: "lightblue"
    property string p_nativeID
    property bool p_secureTextEntry: false
    property color p_borderColor: "black"
    property double p_borderWidth: 0
    property double p_borderRadius: 0
    property color p_backgroundColor: "transparent"
    property double p_fontSize: 14
    property string p_fontFamily
    property string p_fontWeight
    property int p_fontWeightEnum
    property bool p_autoFocus: false
    property var p_submitShortcut: defaultShortcut(p_multiline)

    readonly property int defaultHeight: 21
    property bool heightSetInternally: false

    property var flexbox: React.Flexbox {
        control: textInputRoot;
        viewManager: textInputManager
    }

    function onJsPropertiesSet() {
        if(flexbox) {
            if(flexbox.isUndefined(flexbox.p_height)) {
                flexbox.p_height = textInputRoot.defaultHeight
                heightSetInternally = true;
            }
        }
    }

    property bool jsTextChange: false

    function defaultShortcut(multiline) {
        if(multiline)
            return {key: "", modifiers: []};
        else
            return {key: "Enter", modifiers: []};
    }

    objectName: p_nativeID

    onP_textChanged: {
        if(textInputControl) {
            jsTextChange = true

            if(p_multiline) {
                var oldCursorPos = textInputControl.cursorPosition
                var cursorAtEnd = (textInputControl.textAreaLength === textInputControl.cursorPosition)
                textInputControl.text = p_text

                var cursorPos;
                if(cursorAtEnd)
                    cursorPos = textInputControl.textAreaLength
                else
                    cursorPos = Math.min(oldCursorPos, textInputControl.textAreaLength)

                textInputControl.cursorPosition = cursorPos
            }
            else {
                textInputControl.text = p_text
            }

            jsTextChange = false
        }
    }

    onP_fontWeightChanged: {
        switch(p_fontWeight) {
        case "100": p_fontWeightEnum = Font.Thin; break;
        case "200": p_fontWeightEnum = Font.Light; break;
        case "300": p_fontWeightEnum = Font.ExtraLight; break;
        case "400": p_fontWeightEnum = Font.Normal; break;
        case "500": p_fontWeightEnum = Font.Medium; break;
        case "600": p_fontWeightEnum = Font.DemiBold; break;
        case "700": p_fontWeightEnum = Font.Bold; break;
        case "800": p_fontWeightEnum = Font.ExtraBold; break;
        case "900": p_fontWeightEnum = Font.Black; break;
        case "normal": p_fontWeightEnum = Font.Normal; break;
        case "bold": p_fontWeightEnum = Font.Bold; break;
        default: p_fontWeightEnum = Font.Normal; break;
        }
    }

    function recreateInputControl() {
        if(textInputControl) {
            textInputControl.destroy();
            textInputControl = null;
        }

        var component;
        if(p_multiline) {
            component = Qt.createComponent("ReactTextInputArea.qml");
            textInputControl = component.createObject(textInputRoot);
        }
        else {
            component = Qt.createComponent("ReactTextInputField.qml");
            textInputControl = component.createObject(textInputRoot);
        }
        if(!textInputControl) {
            console.error("Can't load TextInput qml component");
        }
    }

    function keyText(key, text) {
        var t = text
        switch(key) {
        case Qt.Key_Return:
        case Qt.Key_Enter:
            t = "Enter"
            break;
        case Qt.Key_Backspace:
            t = "Backspace"
            break;
        }

        return t;
    }

    function keyModifiers(modifiers) {
        var modArray = [];

        if(modifiers & Qt.ShiftModifier) {
            modArray.push("shift")
        }
        if(modifiers & Qt.ControlModifier) {
            modArray.push("control")
        }
        if(modifiers & Qt.AltModifier) {
            modArray.push("alt")
        }
        if(modifiers & Qt.MetaModifier) {
            modArray.push("meta")
        }

        return modArray;
    }


    Component.onCompleted: recreateInputControl();
    onP_multilineChanged: recreateInputControl();
}
