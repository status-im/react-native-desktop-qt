import QtQuick 2.9
import QtQuick.Controls 2.2
import QtGraphicalEffects 1.0
import React 0.1 as React

Item {

    id: modalRoot
    objectName: "modalRoot"

    property string p_animationType
    property bool p_onShow: false
    property bool p_transparent
    property var contentItem: popup.contentItem
    property var modalManager: null

    property var flexbox: React.Flexbox {
        control: contentItem
        p_minWidth: popup.width
        p_minHeight: popup.height
    }

    Popup {
        id: popup
        width: ApplicationWindow.overlay ? ApplicationWindow.overlay.width : 0
        height: ApplicationWindow.overlay ? ApplicationWindow.overlay.height : 0
        visible: true
        modal: true

        Component.onCompleted: {
            //for convenient debugging
            contentItem.objectName = "ReactModal.contentItem"
        }

        closePolicy: Popup.NoAutoClose
        onOpened: modalManager.sendOnShowNotificationToJs(modalRoot)
    }
}
