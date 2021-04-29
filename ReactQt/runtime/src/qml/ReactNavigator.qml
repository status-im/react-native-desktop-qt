import QtQuick 2.4
import QtQuick.Controls 2.2


Item {
    id: navigatorRoot

    property bool p_onBackButtonPress: false
    property int numberPages: 0

    signal backTriggered();

    Component {
        id: pageBackAction
        // TODO: Action object will appear in Qt Quick Controls 2.3
        // starting from Qt 5.10 release https://doc-snapshots.qt.io/qt5-dev/qml-qtquick-controls2-action.html
        /*Action {
            iconName: navigatorRoot.numberPages > 1 ? "back" : ""
        }*/
        Button {
            text: navigatorRoot.numberPages > 1 ? "back" : ""
        }
    }

    StackView {
        id: pageStack
        anchors.fill: parent
    }

    function push(item) {
        item.head.backAction = pageBackAction.createObject(item);
        // TODO: Revert back along with Action support
        //item.head.backAction.onTriggered.connect(backTriggered);
        item.head.backAction.onClicked.connect(backTriggered);
        pageStack.push(item);
        navigatorRoot.numberPages += 1;
    }

    function pop() {
        pageStack.pop();
        navigatorRoot.numberPages -= 1;
    }

    function clear() {
        pageStack.clear();
    }
}
