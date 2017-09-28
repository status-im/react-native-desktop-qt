import QtQuick 2.9
import QtQuick.Controls 2.2
import QtGraphicalEffects 1.0
import React 0.1 as React

BusyIndicator {
    id: activityIndicatorRoot

    property bool p_animating: false
    property string p_color: "gray"
    property string p_size: 'small' //we don't use it in qml because size controlled from outside
    property var flexbox: React.Flexbox {control: activityIndicatorRoot}

    running: p_animating

    contentItem: Item {
        id: content

        Image {
            id: indicatorImage
            visible: false
            source: "qrc:/images/spinner_medium.png"
        }

        ColorOverlay {
            id: tintEffect
            visible: activityIndicatorRoot.running
            anchors.fill: parent
            source: indicatorImage
            color: activityIndicatorRoot.p_color

            RotationAnimator on rotation {
                running: activityIndicatorRoot.running
                loops: Animation.Infinite
                duration: 4000
                from: 0; to: 360
            }
        }
    }


}
