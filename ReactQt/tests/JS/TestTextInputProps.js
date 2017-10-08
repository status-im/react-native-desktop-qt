
import React, { Component } from 'react';
import {
  AppRegistry,
  TextInput
} from 'react-native';


export default class TextInputReactNative extends Component {
  render() {
    return (
      <TextInput
           testID="textinput"
           value="some text"
           multiline={true}
           selectionColor={'red'}
           placeholder="Enter text to see events"
           onChange={(event) => console.log("TextInput.onChange()")}
           onSelectionChange={(event) => console.log("TextInput.oSelectionChange()")}
           style={[{textAlign: 'left'}, {color: 'green'}]}
        >
        </TextInput>
    );
  }
}

AppRegistry.registerComponent('TestTextInputProps', () => TextInputReactNative)
