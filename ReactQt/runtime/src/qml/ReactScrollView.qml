import QtQuick 2.4
import QtQuick.Controls 1.4

Flickable {
 id: scrollView

 property bool p_onScroll: false

 clip: true
 anchors.fill: parent
 contentHeight: contentItem.childrenRect.height
 contentWidth: contentItem.childrenRect.width
}
