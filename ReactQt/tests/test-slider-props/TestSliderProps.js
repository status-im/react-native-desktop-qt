import React, { Component } from 'react';
import {
  AppRegistry,
  StyleSheet,
  Text,
  View,
  Slider
} from 'react-native';

export default class SliderReactNative extends Component {
  render() {
    return (
      <Slider
        value={0.5}
        step={0.1}
        minimumValue={0}
        maximumValue={1.0}
        minimumTrackTintColor={'green'}
        maximumTrackTintColor={'green'}
        disabled={false}
        onValueChange={() => console.log('Slider.onValueChange()')}
        onSlidingComplete={() => console.log('Slider.onSlidingComplete()')}
        thumbTintColor={'green'}
        testID={'testSlider'}
        />
    );
  }
}

AppRegistry.registerComponent('TestSliderProps', () => SliderReactNative);
