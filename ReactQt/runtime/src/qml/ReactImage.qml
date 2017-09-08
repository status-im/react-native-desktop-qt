import QtQuick 2.4
import QtGraphicalEffects 1.0
import React 0.1 as React

React.Item {
  id: imageRoot
  p_backgroundColor: 'transparent'

  property color p_tintColor
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

  property var imageManager: null
  property string managedSource

  onP_sourceChanged: {
    //Manager will load image and set correct url to "managedSource" property
    imageManager.manageSource(p_source, imageRoot);
  }


  objectName: p_testID

  onP_tintColorChanged: {
    image.visible = false
    colorOverlay.visible = true
  }

  Image {
    id: image
    visible: true
    anchors.fill: parent
    layer.enabled: imageRoot.p_borderRadius > 0
    fillMode: fillModeFromResizeMode(imageRoot.p_resizeMode)
    source: imageRoot.managedSource

    layer.effect: OpacityMask {
      maskSource: Rectangle {
        width: image.width
        height: image.height
        radius: imageRoot.p_borderRadius
      }
    }
  }

  ColorOverlay {
    visible: false
    anchors.fill: image
    id: colorOverlay
    source: image
    color: imageRoot.p_tintColor
  }

  FastBlur {
    anchors.fill: image
    source: image
    radius: p_blurRadius
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
