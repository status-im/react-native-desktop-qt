import QtQuick 2.7
import QtQuick.Controls 2.2
import React 0.1 as React
import  "../js/utils.js" as Utils

Item {
    id: textInputRoot
    property var textInputManager: null
    property var textInputControl: null

    property string p_text
    property color p_color
    property bool p_multiline: false
    property bool p_onChange: false
    property bool p_onSelectionChange: false
    property string p_placeholderText
    property string p_textAlign: "left"
    property string p_selectionColor: "lightblue"
    property string p_testID
    property bool p_secureTextEntry: false

    property var flexbox: React.Flexbox {control: textInputRoot}

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

    Component.onCompleted: recreateInputControl();
    onP_multilineChanged: recreateInputControl();
}
