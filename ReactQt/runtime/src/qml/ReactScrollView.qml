import QtQuick 2.4
import QtQuick.Controls 2.2
import React 0.1 as React

Flickable {
    id: scrollViewRoot

    property var scrollViewManager: null
    property bool p_onScroll: false
    property var flexbox: React.Flexbox {control: scrollViewRoot}
    property bool p_enableArrayScrollingOptimization: false
    property int p_headerHeight: 0
    property int p_footerWidth: 0
    property var p_transform
    property bool invertedScroll: false
    property bool p_inverted: false
    property bool p_showsHorizontalScrollIndicator: true
    property bool p_showsVerticalScrollIndicator: true

    onHeightChanged: {
        if(p_transform)
            scrollViewManager.applyTransformation(scrollViewRoot, p_transform)
    }
    onWidthChanged: {
        if(p_transform)
            scrollViewManager.applyTransformation(scrollViewRoot, p_transform)
    }

    onP_transformChanged: {
        scrollViewManager.applyTransformation(scrollViewRoot, p_transform)
        //if scrollview vertically inveted we should remember that to adjust wheel events
        if(p_transform.length > 5) {
            var scaleY = p_transform[5];
            invertedScroll = (scaleY < 0);
        }
    }


    clip: true
    contentHeight: contentItem.childrenRect.height
    contentWidth: contentItem.childrenRect.width

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
