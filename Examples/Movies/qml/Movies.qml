import React 0.1
import QtQuick 2.4

Rectangle {
  id: root
  width: 640; height: 480;
  color: "red"

  ReactView {
    anchors.fill: parent 

    moduleName: "Movies"
    codeLocation: "http://localhost:8081/Examples/Movies/MoviesApp.ubuntu.bundle?platform=ubuntu&dev=true"
  }
}
