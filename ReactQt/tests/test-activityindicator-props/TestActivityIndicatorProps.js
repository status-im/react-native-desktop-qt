import React, { Component } from 'react';
import {
  AppRegistry,
  ActivityIndicator,
} from 'react-native';

export default class ActivityIndicatorReactNative extends Component {
  render() {
    return (
        <ActivityIndicator  size='large'
                            color='#0000FF'
                            animating={true}
        />
    );
  }
}

AppRegistry.registerComponent('TestActivityIndicatorProps', () => ActivityIndicatorReactNative);
