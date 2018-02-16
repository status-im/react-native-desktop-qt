import QtQuick 2.9
import QtQuick.Controls 2.2


// Dynamic creation of Dialog component from C++ fails, so we wrap it in an Item
Item {
    id: root

    property var buttons
    property var alertManager
    property int callback
    property string title
    property string message
    property bool cancelable: true
    property bool closedByButton: false

    function open() {dialog.open()}

    anchors.fill: parent


    Dialog {
        id: dialog

        width: 400
        height: 200

        title: root.title
        modal: true
        closePolicy: cancelable ? Popup.CloseOnPressOutside : Popup.NoAutoClose

        onClosed: {
            root.visible = false
            if(!root.closedByButton) {
                root.alertManager.sendDismissedToJs(callback)
            }
        }

        contentItem: Text {
            anchors {
                top: dialog.header.bottom
                bottom: dialog.footer.top
                left: parent.left
                right: parent.right
                margins: 20
            }
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            text: root.message
            wrapMode: Text.WordWrap
        }

        footer: DialogButtonBox {

            Repeater {
                model: buttons
                delegate:  Button {
                    text: buttons[index].text
                    property var name: buttons[index].name
                    DialogButtonBox.buttonRole: buttonRole(buttons[index].role)
                 }
            }
            onClicked: {
                console.log("clicked")
                root.closedByButton = true;
                root.alertManager.sendButtonClickToJs(callback, button.name)
                dialog.accept();
            }
        }
    }

    function buttonRole(roleName) {
        if(roleName === "accept")
            return DialogButtonBox.AcceptRole;
        if(roleName === "reject")
            return DialogButtonBox.RejectRole;

        return DialogButtonBox.ApplyRole;
    }
}
