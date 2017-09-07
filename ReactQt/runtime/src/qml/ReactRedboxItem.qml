import QtQuick 2.4
import QtQuick.Controls 1.4

Rectangle {
 id: redboxRoot
 color: 'red'
 anchors.fill: parent

 property alias message: textMessage.text
 property alias stackModel: stackListView.model

 signal dismissPressed();
 signal reloadPressed();

 Text {
   id: textMessage
   visible: message.length > 0
   anchors {
     centerIn: redboxRoot
     margins: 40
   }
   color: 'white'
   font.pointSize: 18
   wrapMode: Text.WordWrap
   onTextChanged: console.log("redbox message:", text)
 }

 ListView {
   id: stackListView
   anchors {
     top: textMessage.bottom
     left: parent.left
     bottom: buttonRow.top
     right: parent.right
     margins: 40
   }

   clip: true

   delegate: Label {
       height: 60
       color: 'white'
       text: methodName + "\n" + file + " @ " + lineNumber + ":" + column
       font.weight: Font.DemiBold
       verticalAlignment: Text.AlignVCenter
     }
   }

 Row {
   id: buttonRow
   anchors {
     bottomMargin: 60
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

 state: stackModel !== undefined && !stackModel.empty ? "stackTrace" : "errorMessage"
 onStateChanged: {
   // XXX: weird hack
   if (state === 'stackTrace') {
     textMessage.anchors.centerIn = undefined;
   }
 }

 states: [
   State {
     name: "errorMessage"
     PropertyChanges {
       target: textMessage
       anchors.left: undefined
       anchors.top: undefined
       anchors.centerIn: redboxRoot
     }
     PropertyChanges {
       target: stackListView
       visible: false
     }
   },
   State  {
     name: "stackTrace"
     PropertyChanges {
       target: textMessage
       anchors.centerIn: undefined
       anchors.left: redboxRoot.left
       anchors.top: redboxRoot.top
     }
     PropertyChanges {
       target: stackListView
       visible: true
     }
   }
 ]
}
