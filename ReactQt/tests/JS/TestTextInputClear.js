import React, { Component } from 'react';
import {
  AppRegistry,
  View,
  NativeButton,
  TextInput
} from 'react-native';

export default class TestTextInputClear extends Component {

   render() {
      return (
        <View nativeID={"topView"}>
            <TextInput nativeID={"textInput"} defaultValue={"Hello"} ref={input => { this.textInput = input }} style={{height: 50, width: 200}}/>
            <NativeButton testID={"clearButton"} title={"Clear"} onPress={()=>{this.textInput.clear()}}></NativeButton>
        </View>
      )
   }
}


AppRegistry.registerComponent('TestTextInputClear', () => TestTextInputClear)
