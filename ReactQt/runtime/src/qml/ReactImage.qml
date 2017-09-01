import QtQuick 2.4
import QtGraphicalEffects 1.0
import React 0.1 as React

React.Item {
  id: imageRect
  backgroundColor: 'transparent'

  property alias resizeMode: image.fillMode
  property alias tintColor: colorOverlay.color
  property string prop_testID

  property var p_source;
  property bool p_onLoadStart: false
  property bool p_onLoadEnd: false
  property bool p_onLoad: false
  property bool p_onError: false
  property bool p_onProgress: false
  property bool p_onLayout: false
  property double p_blurRadius: 0

  property var imageManager: null
  property string cachedSource

  onP_sourceChanged: {
    //Manager will load image and set "cachedSource" property to a new url
    imageManager.loadSourceForImage(p_source, imageRect);
  }

  objectName: prop_testID

  onTintColorChanged: {
    image.visible = false
    colorOverlay.visible = true
  }



  Image {
    id: image
    visible: true
    anchors.fill: parent
    layer.enabled: imageRect.borderRadius > 0
    fillMode: Image.PreserveAspectCrop
    source: imageRect.cachedSource

    layer.effect: OpacityMask {
      maskSource: Rectangle {
        width: image.width
        height: image.height
        radius: imageRect.borderRadius
      }
    }
  }
  ColorOverlay {
    visible: false
    anchors.fill: image
    id: colorOverlay
    source: image
  }
}
