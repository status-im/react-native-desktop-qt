import QtQuick 2.9
import QtQuick.Controls 2.2
import QtGraphicalEffects 1.0
import React 0.1 as React

Button {
    id: buttonRoot

    property string p_accessibilityLabel
    property string p_title
    property string p_color: "#e0e0e0"
    property bool p_disabled: false
    property bool p_onPress: false
    property string p_testID
    property var buttonManager: null
    property var flexbox: React.Flexbox {
        control: buttonRoot
        p_minWidth: buttonRoot.contentItem.contentWidth + 2*priv.textMargin
        p_minHeight: buttonRoot.contentItem.contentHeight + 2*priv.textMargin
    }


    QtObject {
        id: priv
        property int textMargin: 5
    }

    text: p_title
    objectName: p_testID
    enabled: !p_disabled

    Component.onCompleted: {
        buttonRoot.contentItem.elide = Text.ElideNone
    }

    background: Rectangle {
        color: buttonRoot.down ? Qt.darker(buttonRoot.p_color, 1.2) : buttonRoot.p_color
        visible: !buttonRoot.flat || buttonRoot.down || buttonRoot.checked || buttonRoot.highlighted
    }
    onPressed: buttonManager.sendPressedNotificationToJs(buttonRoot)
}
