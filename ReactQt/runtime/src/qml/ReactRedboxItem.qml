import QtQuick 2.4
import QtQuick.Controls 2.2

Rectangle {
    id: redboxRoot
    anchors.fill: parent
    color: "#bd271a"

    property var stackmodel
    property string message
    property alias stackModel: listView.model

    signal dismissPressed();
    signal reloadPressed();


    ListView {
        id: listView

        spacing: 15
        clip: true
        visible: stackModel ? (!stackModel.empty) : false

        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
            bottom: buttonRow.top
            margins: 20
        }

        header: Text {
            id: textMessage
            width: parent.width

            visible: message.length > 0
            height: contentHeight + 30
            text: redboxRoot.message

            color: 'white'
            font.pointSize: 16
            font.weight: Font.DemiBold
            wrapMode: Text.WordWrap
        }

        delegate: Label {
            height: contentHeight
            anchors {
                left: parent.left
                right: parent.right
            }

            color: 'white'
            text: methodName + "\n" + file + " @ " + lineNumber + ":" + column

            font.pointSize: 14
            font.family: 'Courier'
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WordWrap
        }
    }

    Text {
        visible: stackModel ? stackModel.empty : false
        text: redboxRoot.message

        anchors {
            centerIn: parent
        }
        color: 'white'
        font.pointSize: 16
        font.weight: Font.DemiBold
        wrapMode: Text.WordWrap
    }

    Row {
        id: buttonRow
        anchors {
            bottomMargin: 10
            horizontalCenter: parent.horizontalCenter
            bottom: parent.bottom
        }

        spacing: 40

        Button {
            text: 'Reload JS'
            onClicked: reloadPressed()
        }

        Button {
            text: 'Dismiss'
            onClicked: dismissPressed()
        }
    }
}
