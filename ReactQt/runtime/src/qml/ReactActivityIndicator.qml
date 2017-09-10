import QtQuick 2.9
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0

BusyIndicator {
    id: activityIndicatorRoot

    property bool p_animating: false
    property string p_color: "gray"
    property string p_size: 'small' //we don't use it in qml because size controlled from outside

    running: p_animating

    style: BusyIndicatorStyle {
        indicator: Item {

            property bool isRunning: control.running

            Image {
                id: indicatorImage
                visible: false
                source: "qrc:/images/spinner_medium.png"
            }

            ColorOverlay {
                id: tintEffect
                visible: isRunning
                anchors.fill: parent
                source: indicatorImage
                color: activityIndicatorRoot.p_color

                RotationAnimator on rotation {
                    running: isRunning
                    loops: Animation.Infinite
                    duration: 4000
                    from: 0; to: 360
                }
            }
        }
    }

}
