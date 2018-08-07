import QtQuick 2.4
import QtQuick.Controls 2.2
import React 0.1 as React

ListView{
    id: scrollViewRoot

    property var scrollViewManager: null
    property bool p_onScroll: false
    property var flexbox: React.Flexbox {control: scrollViewRoot; viewManager: scrollViewManager}
    property bool p_enableArrayScrollingOptimization: false
    property int p_headerHeight: 0
    property int p_footerWidth: 0

    clip: true
    contentHeight: contentItem.childrenRect.height
    contentWidth: contentItem.childrenRect.width

    onCountChanged: {
        if(scrollViewManager)
            scrollViewManager.sendOnLayoutToJs(scrollViewRoot, contentX, contentY, contentWidth, contentHeight);
    }

    header: Item {
        height: p_headerHeight
    }
    footer: Item {
        height: p_footerWidth
    }

    delegate: Item {
        id: componentId
        height: modelData.height
        width: modelData.width
        Component.onCompleted: {
            modelData.parent = componentId
            modelData.anchors.centerIn = componentId
        }

        Component.onDestruction: {
            modelData.parent = null
        }
    }
}
