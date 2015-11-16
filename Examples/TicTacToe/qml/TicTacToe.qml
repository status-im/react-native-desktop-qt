import React 1.0
import QtQuick 2.4

Rectangle {
  id: root
  width: 640; height: 480;
  color: "red"

  ReactView {
    anchors.fill: parent 

    moduleName: "TicTacToeApp"
    codeLocation: "http://localhost:8081/Examples/TicTacToe/TicTacToeApp.bundle?platform=ubuntu&dev=true"
  }
}

