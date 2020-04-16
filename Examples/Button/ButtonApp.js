
import React, { Component } from 'react';
import {
  AppRegistry,
  StyleSheet,
  Button,
  View,
  Text
} from 'react-native';

import MessageQueue from 'react-native/Libraries/BatchedBridge/MessageQueue.js';

const spyFunction = (msg) => {
  console.log(msg);
};

MessageQueue.spy(spyFunction);


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
      <Button
           onPress={this.changeButtonColor}
           title="Click me"
           color={this.state.buttonColor}
           accessibilityLabel="Learn more about this purple button"
        >
        </Button>
    );
  }
}

AppRegistry.registerComponent('ButtonApp', () => ButtonReactNative)
