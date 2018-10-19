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
    property bool p_inverted: false
    property bool p_showsHorizontalScrollIndicator: true
    property bool p_showsVerticalScrollIndicator: true

    clip: true
    highlightFollowsCurrentItem: false

    onCountChanged: {
        if(scrollViewManager)
            scrollViewManager.sendOnLayoutToJs(scrollViewRoot,
                                               contentX,
                                               contentY,
                                               contentItem.childrenRect.width,
                                               contentItem.childrenRect.height);
    }

    verticalLayoutDirection: p_inverted ? ListView.BottomToTop : ListView.TopToBottom

    header: Item {
        height: p_headerHeight
    }
    footer: Item {
        height: p_footerWidth
    }

    delegate: Item {
        id: componentId
        height: model.display.height
        width: model.display.width
        Component.onCompleted: {
            model.display.parent = componentId
            model.display.anchors.centerIn = componentId
        }
    }

    onFlickingChanged: {
        if(scrollViewManager) {
            if(flicking) {
                scrollViewManager.momentumScrollBegin(scrollViewRoot);
            } else {
                scrollViewManager.momentumScrollEnd(scrollViewRoot);
            }
        }
    }

    ScrollBar.horizontal: ScrollBar { 
        policy: p_showsHorizontalScrollIndicator ? ScrollBar.AsNeeded : ScrollBar.AlwaysOff;
    }

    ScrollBar.vertical: ScrollBar { 
        policy: p_showsVerticalScrollIndicator ? ScrollBar.AsNeeded : ScrollBar.AlwaysOff;
    }

}
