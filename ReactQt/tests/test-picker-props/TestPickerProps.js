import React, { Component } from 'react';
import {
  AppRegistry,
  Picker
} from 'react-native';

const StyleSheet = require('StyleSheet');
const Item = Picker.Item;

export default class PickerReactNative extends Component {
  render() {
    return (
        <Picker 
           style={styles.picker} 
           testID="picker"
           selectedValue="green"
           onValueChange={(val, pos) => console.log("Picker.onValueChange()")} 
         >
           <Item label="red" value="red" />
           <Item label="green" value="green" />
        </Picker>
    );
  }
}

var styles = StyleSheet.create({
  picker: {
    width: 100,
    height: 200
  },
});

AppRegistry.registerComponent('TestPickerProps', () => PickerReactNative)


