import QtQuick 2.9
import QtQuick.Controls 2.2
import QtGraphicalEffects 1.0
import React 0.1 as React

Slider {
    id: sliderRoot

    property var sliderManager: null

    property double p_value
    property double p_step
    property double p_minimumValue
    property double p_maximumValue
    property string p_minimumTrackTintColor
    property string p_maximumTrackTintColor
    property bool p_disabled: false
    property string p_thumbTintColor
    property string p_testID
    property var flexbox: React.Flexbox {
        control: sliderRoot
        p_minWidth: sliderRoot.implicitWidth
        p_minHeight: sliderRoot.implicitHeight
    }

    objectName: p_testID

    value: p_value
    stepSize: p_step
    from: p_minimumValue
    to: p_maximumValue
    enabled: !p_disabled

    onValueChanged: {
        sliderManager.sendSliderValueChangedToJs(sliderRoot)
    }

    onPressedChanged: {
        if (!pressed)
            sliderManager.sendSlidingCompleteToJs(sliderRoot)
    }
}
