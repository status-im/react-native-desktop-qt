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

        //position dialog window in the center
        x: root.x + root.width/2 - dialog.width/2
        y: root.y + root.height/2 - dialog.height/2

        width: maxWidth < alertWidth ? maxWidth : alertWidth

        property int maxWidth: root.width - 20
        property int alertWidth: Math.max(titleWidth, textWidth, buttonsWidth) + 2*contentText.anchors.margins
        property int textWidth: textFontMetrics.boundingRect(message).width
        property int titleWidth: titleFontMetrics.boundingRect(title).width
        property int buttonsWidth: root.buttons ? root.buttons.length * 100 : 0


        FontMetrics {
            id: titleFontMetrics
            font: dialog.header.font
        }

        FontMetrics {
            id: textFontMetrics
            font: contentText.font
        }

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
            id: contentText
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

        footer: createButtonBox(buttons)

        function createButtonBox(buttons) {
            return Qt.createQmlObject('
                                    import QtQuick 2.4
                                    import QtQuick.Controls 2.2
                                    DialogButtonBox {
                                        Repeater {
                                            model: buttons
                                            delegate:  Button {
                                                text: buttons[index].text
                                                property var name: buttons[index].name
                                                DialogButtonBox.buttonRole: buttonRole(buttons[index].role)
                                            }
                                         }
                                         onClicked: {
                                             root.closedByButton = true;
                                             root.alertManager.sendButtonClickToJs(callback, button.name)
                                             dialog.accept();
                                         }
                                     }',dialog, "dynamicButtonBox");
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
