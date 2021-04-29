
import QtQuick 2.7
import QtQuick.Controls 2.2

Rectangle {
    id: devMenuRootId
    anchors.fill: parent
    color: "transparent"
    property var rootView: null

    function hideDevMenu() {
        devMenuRootId.state = "devMenuHidden"
    }

    Rectangle {
        id: devMenuId
        anchors.top: parent.bottom
        anchors.margins: 50
        anchors.horizontalCenter: parent.horizontalCenter
        color: "grey"
        opacity: 0.8
        width: devMenuColumnId.width + 100
        height: devMenuColumnId.height + 100
        Column {
            id: devMenuColumnId
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            spacing: 20
            Button {
                text: "Reload"
                highlighted: true
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottomMargin: 10
                onClicked: {
                    hideDevMenu()
                    rootView.reloadBridge()
                }
            }
            Button {
                text: "Remote JS Debugging"
                highlighted: true
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottomMargin: 10
                onClicked: {
                    hideDevMenu()
                    rootView.startRemoteJSDebugging()
                }
            }
            Button {
                text: rootView.liveReload ? "Disable Live Reload" : "Enable Live Reload"
                highlighted: true
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottomMargin: 10
                onClicked: {
                    hideDevMenu()
                    rootView.liveReload = !rootView.liveReload
                }
            }
            Button {
                text: rootView.hotReload ? "Disable Hot Reloading" : "Enable Hot Reloading"
                highlighted: true
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottomMargin: 10
                onClicked: {
                    hideDevMenu()
                    rootView.hotReload = !rootView.hotReload
                }
            }
            Button {
                text: "Cancel"
                highlighted: true
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottomMargin: 10
                onClicked: hideDevMenu()
            }
        }
    }


    states: [
        State {
            name: "devMenuVisible"
            AnchorChanges { target: devMenuId; anchors.top: devMenuRootId.top; anchors.verticalCenter: devMenuRootId.verticalCenter }
            PropertyChanges {
                target: devMenuId
                visible: true
            }
        },
        State {
            name: "devMenuHidden"
            AnchorChanges { target: devMenuId; anchors.top: devMenuRootId.bottom; anchors.verticalCenter: undefined }
            PropertyChanges {
                target: devMenuId
                visible: false
            }
        }
    ]

    transitions: Transition {
        // devMenuId reanchoring animation
        AnchorAnimation { duration: 200 }
    }

    Shortcut {
        sequence: "Ctrl+R"
        onActivated: devMenuRootId.state = "devMenuVisible"
    }
}
