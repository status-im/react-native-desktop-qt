import React, { Component } from 'react';
import {
  AppRegistry,
  ActivityIndicator,
} from 'react-native';

export default class ActivityIndicatorReactNative extends Component {
  render() {
    return (
        <ActivityIndicator size='large' color='#00FF00'>
        </ActivityIndicator>
    );
  }
}

AppRegistry.registerComponent('ActivityIndicatorApp', () => ActivityIndicatorReactNative)
