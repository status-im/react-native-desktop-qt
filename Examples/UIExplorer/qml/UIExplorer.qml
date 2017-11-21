
import QtQuick 2.4
import React 0.1 as React

Rectangle {
  id: root
  width: 640; height: 480;
  color: "red"

  React.RootView {
    anchors.fill: parent

    moduleName: "UIExplorerApp"
    codeLocation: "http://localhost:8081/Examples/UIExplorer/UIExplorerApp.desktop.bundle?platform=desktop&dev=true"
  }
}
