import QtQuick 2.9
import QtQuick.Controls 2.2
import QtGraphicalEffects 1.0
import React 0.1 as React

Item {

    id: modalRoot
    objectName: "modalRoot"

    property string p_animationType
    property bool p_onShow: false
    property bool p_transparent: false
    property var contentItem: popup.contentItem
    property var modalManager: null

    property var flexbox: React.Flexbox {
        control: popup.contentItem
        p_minWidth: popup.width
        p_minHeight: popup.height
    }

    Popup {
        id: popup
        width: ApplicationWindow.overlay ? ApplicationWindow.overlay.width : 0
        height: ApplicationWindow.overlay ? ApplicationWindow.overlay.height : 0
        visible: true
        modal: true
        margins: 0

        background: Item {}

        contentItem: React.MouseEventsInterceptor {
            Rectangle {
                objectName: "ReactModal.contentItem"
                color: modalRoot.p_transparent ? "transparent" : "white"
            }
        }

        closePolicy: Popup.NoAutoClose
        onOpened: modalManager.sendOnShowNotificationToJs(modalRoot)
    }
}
