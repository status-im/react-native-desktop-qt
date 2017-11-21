
import QtQuick 2.4
import React 0.1 as React

Rectangle {
  id: root
  width: 640; height: 480;

  React.RootView {
    anchors.fill: parent

    moduleName: "Game2048"
    codeLocation: "http://localhost:8081/Examples/2048/Game2048.bundle?platform=desktop&dev=true"
  }
}
