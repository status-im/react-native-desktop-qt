import QtQuick 2.7
import QtQuick.Controls 2.2
import React 0.1 as React
import QtQuick.Extras 1.4
import QtQuick.Controls.Styles 1.2


Tumbler {
    id: tumbler
    property var pickerManager: null
    property variant flexbox: React.Flexbox {control: tumbler}
    property variant p_items: []
    property int p_selected:0
    property bool p_onValueChange: false
    property string p_testID

    objectName: p_testID

    TumblerColumn {
        model: p_items
        onCurrentIndexChanged: {
            if (pickerManager) {
                pickerManager.sendValueChangeToJs(tumbler, currentIndex);
            }
        }
    }

    style: TumblerStyle {
        id: tumblerStyle

        delegate: Item {
            implicitHeight: (tumbler.height - padding.top - padding.bottom) / tumblerStyle.visibleItemCount
            Text {
                id: label
                text: styleData.value.label
                color: styleData.value.color ? styleData.value.color : "black"
                opacity: 0.4 + Math.max(0, 1 - Math.abs(styleData.displacement)) * 0.6
                anchors.centerIn: parent
            }
        }
    }

    onP_selectedChanged: {
        tumbler.setCurrentIndexAt(0, p_selected);
    }
}
