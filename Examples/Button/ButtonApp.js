
import React, { Component } from 'react';
import {
  AppRegistry,
  StyleSheet,
  Button,
  View,
  Text,
  Platform
} from 'react-native';


export default class ButtonReactNative extends Component {

    constructor(props) {
      super(props);
      this.state = {
        buttonText: 'red' // default button color goes here
      };
    }

    changeButtonText = () => {
        if(Platform.isMacOs){
            this.setState({buttonText: 'macos'})
        }
        else{
            this.setState({buttonText: 'unknown'})
        }
      }

  render() {
    return (
      <Button
           onPress={this.changeButtonText}
           title={this.state.buttonText}
           accessibilityLabel="Learn more about this purple button"
        >
        </Button>
    );
  }
}

AppRegistry.registerComponent('ButtonApp', () => ButtonReactNative)
