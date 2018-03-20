import React, { Component } from 'react';
import {
  AppRegistry,
  View,
  Button,
  TextInput
} from 'react-native';

export default class TestTextInputClear extends Component {

   render() {
      return (
        <View nativeID={"topView"}>
            <TextInput nativeID={"textInput"} defaultValue={"Hello"} ref={input => { this.textInput = input }} style={{height: 50, width: 200}}/>
            <Button testID={"clearButton"} title={"Clear"} onPress={()=>{this.textInput.clear()}}></Button>
        </View>
      )
   }
}


AppRegistry.registerComponent('TestTextInputClear', () => TestTextInputClear)
