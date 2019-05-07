import QtQuick 2.7
import QtGraphicalEffects 1.0
import React 0.1 as React

React.Item {
    id: imageRoot
    p_backgroundColor: 'transparent'

    property color p_tintColor: 'transparent'
    property string p_testID
    property var p_source;
    property bool p_onLoadStart: false
    property bool p_onLoadEnd: false
    property bool p_onLoad: false
    property bool p_onError: false
    property bool p_onProgress: false
    property bool p_onLayout: false
    property double p_blurRadius: 0
    property string p_resizeMode: 'cover'
    property bool isSVG: false

    property var imageManager: null
    property string managedSource

    property var flexbox: React.Flexbox {control: imageRoot}

    //It appeared that closing application while image still loading its source
    //leads to a crash. So in quick tests we can use this property to prevent closing
    //until image source loaded
    property bool imageReady: image.status != Image.Loading

    onP_sourceChanged: {
        //Manager will load image and set correct url to "managedSource" property
        imageManager.manageSource(p_source, imageRoot);
    }

    objectName: p_testID

    Image {
        id: image
        anchors.fill: parent
        fillMode: fillModeFromResizeMode(imageRoot.p_resizeMode)
        source: imageRoot.managedSource
        visible: false //image not visible, because it is followed by effects and last effect is visible

        //svg images sometimes contain internal height and width specification and gets
        //blurry when resized. To avoid this we set sourceSize that supercedes internal svg settings
        sourceSize.width: isSVG ? flexbox.p_width : undefined
        sourceSize.height: isSVG ? flexbox.p_height : undefined
        asynchronous: true
    }

    FastBlur {
        id: blurEffect
        visible: false
        anchors.fill: image
        source: image
        radius: imageRoot.p_blurRadius
    }

    OpacityMask {
        id: borderRadiusEffect
        anchors.fill: image
        source: blurEffect
        maskSource: Rectangle {
            width: image.width
            height: image.height
            visible: false
            radius: imageRoot.p_borderRadius
        }
    }

    ColorOverlay {
        id: tintEffect
        anchors.fill: image
        source: borderRadiusEffect
        color: imageRoot.p_tintColor
    }


    function fillModeFromResizeMode(resizeMode) {
        switch (resizeMode) {
        case "cover":   return Image.PreserveAspectCrop;
        case "contain": return Image.PreserveAspectFit;
        case "stretch": return Image.Stretch;
        case "repeat":  return Image.Tile;
        case "center":  return Image.Center;
        default:        return Image.PreserveAspectCrop;
        }
    }
}
