import QtQuick 2.4
import QtQuick.Controls 1.4


Item {
 id: root

 property bool p_onBackButtonPress: false

 property int numberPages: 0

 signal backTriggered();

 Component {
   id: pageBackAction
   Action {
     iconName: root.numberPages > 1 ? "back" : ""
   }
 }

 StackView {
   id: pageStack
   anchors.fill: parent
 }

 function push(item) {
   item.head.backAction = pageBackAction.createObject(item);
   item.head.backAction.onTriggered.connect(backTriggered);
   pageStack.push(item);
   root.numberPages += 1;
 }

 function pop() {
   pageStack.pop();
   root.numberPages -= 1;
 }

 function clear() {
   pageStack.clear();
 }
}
