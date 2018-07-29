import QtQuick 2.9
import QtQuick.Controls 2.2
import React 0.1 as React
import QtQuick.Controls.impl 2.2

Switch {
    id: switchItem

    property bool p_value: false
    property var switchManager: null
    property variant flexbox: React.Flexbox {control: switchItem}
    property string p_testID
    property bool p_disabled: false
    property string p_onTintColor: "#353637"
    property string p_tintColor: "#e0e0e0"
    property string p_thumbTintColor

    enabled: !p_disabled

    property bool sendCheckedUpdate: true
    property bool updateCheckedWithValue: true

    indicator: Item {
        x: text ? (switchItem.mirrored ? switchItem.width - width - switchItem.rightPadding : switchItem.leftPadding) : switchItem.leftPadding + (switchItem.availableWidth - width) / 2
        y: switchItem.topPadding + (switchItem.availableHeight - height) / 2
        implicitWidth: 56
        implicitHeight: 28

        Rectangle {
            y: parent.height / 2 - height / 2
            width: 56
            height: 16
            radius: 8
            color: switchItem.checked ? p_onTintColor : p_tintColor
            border.width: switchItem.visualFocus ? 2 : 0
            border.color: Default.focusColor
        }

        Rectangle {
            x: Math.max(0, Math.min(parent.width - width, switchItem.visualPosition * parent.width - (width / 2)))
            y: (parent.height - height) / 2
            width: 28
            height: 28
            radius: 16
            color: p_thumbTintColor ? p_thumbTintColor :(switchItem.enabled ? (switchItem.down
                ? (switchItem.visualFocus ? Default.focusPressedColor : Default.indicatorPressedColor)
                : (switchItem.visualFocus ? Default.focusLightColor : Default.backgroundColor)) : Default.indicatorDisabledColor)
            border.width: switchItem.visualFocus ? 2 : 1
            border.color: switchItem.enabled ? (switchItem.visualFocus
                ? Default.focusColor
                : (switchItem.down ? Default.indicatorFramePressedColor : Default.indicatorFrameColor)) : Default.indicatorFrameDisabledColor

            Behavior on x {
                enabled: !switchItem.down
                SmoothedAnimation { velocity: 200 }
            }
        }
    }


    onP_valueChanged: {
        if(updateCheckedWithValue) {
            sendCheckedUpdate = false;
            checked = p_value;
            sendCheckedUpdate = true;
        }
    }

    onCheckedChanged: {
        if (switchManager) {
            if(sendCheckedUpdate) {
                updateCheckedWithValue = false;
                p_value = checked;
                updateCheckedWithValue = true;
                switchManager.sendValueChangeToJs(switchItem, checked);
            }
        }
    }
}
