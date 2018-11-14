
import React, { Component } from 'react';
import {
  AppRegistry,
  View,
  Text,
  TextInput,
  NativeButton
} from 'react-native';


export default class FetchReactNative extends Component {
  state = {response: "...", url: "https://api.github.com/gists"};

_fetchData = () => {
  fetch(this.state.url)
    .then((response) => this.setState({response: JSON.stringify(response)}))
    .catch((error) => {
      this.setState({response: error})
    });

}
render() {
  return (
    <View>
      <TextInput
        defaultValue={this.state.url}
        style={{height: 50}}
        onChangeText={(text) => this.setState({url: text})}/>
      <NativeButton
        onPress={this._fetchData}
        title="Fetch data"
      />
      <Text>Response:</Text>
      <TextInput value={this.state.response} style={{height: 300}} multiline={true}></TextInput>
    </View>
      );
}
}

AppRegistry.registerComponent('FetchApp', () => FetchReactNative)
