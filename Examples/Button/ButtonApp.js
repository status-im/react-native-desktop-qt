
import React, { Component } from 'react';
import {
  AppRegistry,
  StyleSheet,
  Button
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
      <Button
          onPress={this.changeButtonColor}
          title="Click me"
          color={this.state.buttonColor}
          accessibilityLabel="Learn more about this purple button"
        />
    );
  }
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
    backgroundColor: '#F5FCFF',
  },
  welcome: {
    fontSize: 20,
    textAlign: 'center',
    margin: 10,
  },
  instructions: {
    textAlign: 'center',
    color: '#333333',
    marginBottom: 5,
  },
});

AppRegistry.registerComponent('ButtonApp', () => ButtonReactNative)
