
import React, { Component } from 'react';
import {
  AppRegistry,
  StyleSheet,
  Button,
  View,
  Text,
  Image,
  TextInput,
  FlatList,
  TouchableOpacity,
  TouchableHighlight
} from 'react-native';

var userData = [];
for (var i = 1; i <= 500; i++) {
   var numberedTitle = "Button "+ i;
   userData.push({id: i, title: numberedTitle});
}


class MyListItem extends React.PureComponent {
  _onPress = () => {
    this.props.onPressItem(this.props.id);
  };

  render() {

    console.log("---- RENDER LISTITEM")

    const textColor = this.props.selected ? "red" : "black";
    return (

      <TouchableHighlight onPress={this._onPress} style={{height: 50, backgroundColor: "lightyellow"}}>
        <View style={{flexDirection: "row"}}>
          <Image style={{height: 50, width: 50}} source={{uri: "https://facebook.github.io/react-native/img/header_logo.png"}}>
          </Image>
          <Text style={{ color: textColor }}>
            {this.props.title}
          </Text>
        </View>
      </TouchableHighlight>
    );
  }
}

export default class FlatListReactNative extends Component {
  state = {selected: (new Map(): Map<string, boolean>),
  data: userData,
  height: 50};

  _keyExtractor = (item, index) => item.id.toString();

  _onPressItem = (id: string) => {
    // updater functions are preferred for transactional updates
    this.setState((state) => {
      // copy the map rather than modifying state.
      const selected = new Map(state.selected);
      selected.set(id, !selected.get(id)); // toggle
      return {selected};
    });
  };

_renderItem = ({item}) => (
  <MyListItem
    id={item.id}
    onPressItem={this._onPressItem}
    selected={!!this.state.selected.get(item.id)}
    title={item.title}
    debug={true}
  />
);

updateSize = (height) => {
  this.setState({
    height
  });
}

render() {

  console.log("---- RENDER FLATLIST")

  let newStyle = {
      height: this.state.height
    }

  return (
    <View style={{backgroundColor: "lightRed", flex: 1}} >
        {/*<View style={{height: 50, backgroundColor: "lightgreen"}}>*/}
        {/*<Text> header </Text>*/}
        {/*</View>*/}
        <FlatList
          data={this.state.data}
          extraData={this.state}
          keyExtractor={this._keyExtractor}
          renderItem={this._renderItem}
          scrollEventThrottle={0}
        />
        {/*<TextInput defaultValue="Footer text input"*/}
        {/*style={[newStyle]}*/}
        {/*multiline={true}*/}
        {/*onContentSizeChange={(e) => this.updateSize(e.nativeEvent.contentSize.height)}*/}
        {/*>*/}
        {/*</TextInput>*/}
    </View>
      );
}
}

AppRegistry.registerComponent('FlatListApp', () => FlatListReactNative)
