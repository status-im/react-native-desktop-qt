import QtQuick 2.4
import QtQuick.Controls 2.2
import React 0.1 as React

Flickable {
    id: scrollViewRoot

    property bool p_onScroll: false
    property var flexbox: React.Flexbox {control: scrollViewRoot}

    clip: true
    contentHeight: contentItem.childrenRect.height
    contentWidth: contentItem.childrenRect.width
}
