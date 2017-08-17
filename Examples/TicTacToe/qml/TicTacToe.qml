
import QtQuick 2.4
import React 0.1 as React

Rectangle {
  id: root
  width: 640; height: 480;
  color: "red"

  React.RootView {
    objectName: "rootView"
    anchors.fill: parent

    moduleName: "TicTacToeApp"
    codeLocation: "http://localhost:8081/Examples/TicTacToe/TicTacToeApp.bundle?platform=ubuntu&dev=true"
  }
}
