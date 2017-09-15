import QtQuick 2.9
import QtQuick.Controls 2.2
import QtGraphicalEffects 1.0

Button {
    id: buttonRoot

    property string p_accessibilityLabel
    property string p_title
    property string p_color
    property bool p_disabled: false
    property bool p_onPress: false
    property string p_testID

    property var buttonManager: null

    text: p_title
    objectName: p_testID

    background: Rectangle {
        anchors.fill: parent
        color: buttonRoot.p_color
    }

    onPressed: buttonManager.sendPressedNotificationToJs(buttonRoot)
}
