import QtQuick 2.9
import QtQuick.Controls 2.2
import React 0.1 as React

Tumbler {
    id: tumbler
    property var pickerManager: null
    property variant flexbox: React.Flexbox {control: tumbler}
    property variant p_items: []
    property int p_selected:0
    property bool p_onValueChange: false
    property string p_testID

    model: p_items
    currentIndex: p_selected

    background: Rectangle {
        color: "lightgrey"
        border.color: "grey"
    }
    delegate:
        Text {
        id: label
        text: modelData.label
        color:  modelData.color ? modelData.color : "black"
        font: tumbler.font
        opacity: (1.0 - Math.abs(Tumbler.displacement) / (visibleItemCount / 2)) * (tumbler.enabled ? 1 : 0.6)
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
    onCurrentIndexChanged: {
        if (pickerManager) {
            pickerManager.sendValueChangeToJs(tumbler, currentIndex);
        }
    }
}
