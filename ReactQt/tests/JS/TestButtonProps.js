
import React, { Component } from 'react';
import {
  AppRegistry,
  StyleSheet,
  NativeButton,
  View,
  Text
} from 'react-native';


export default class ButtonReactNative extends Component {

    constructor(props) {
      super(props);
      this.state = {
        buttonColor: 'red' // default button color goes here
      };
    }

    changeButtonColor = () => {
        if(this.state.buttonColor === 'red'){
            this.setState({buttonColor: 'green'})
        }
        else{
            this.setState({buttonColor: 'red'})
        }
      }

  render() {
    return (
      <NativeButton
           onPress={this.changeButtonColor}
           title="Click me"
           color={this.state.buttonColor}
           accessibilityLabel="Accessibility label"
           style={{width:120, height: 50}}
           disabled={false}
           testID="button"
        >
        </NativeButton>
    );
  }
}

AppRegistry.registerComponent('TestButtonProps', () => ButtonReactNative)
