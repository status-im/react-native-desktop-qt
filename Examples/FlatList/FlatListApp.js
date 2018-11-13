
import React, { Component } from 'react';
import {
  AppRegistry,
  StyleSheet,
  Button,
  View,
  Text,
  TextInput,
  FlatList,
  TouchableOpacity
} from 'react-native';

var userData = [];
for (var i = 1; i <= 20; i++) {
   var numberedTitle = "Button "+ i;
   userData.push({id: i, title: numberedTitle});
}


class MyListItem extends React.PureComponent {
  _onPress = () => {
    this.props.onPressItem(this.props.id);
  };

  render() {
    const textColor = this.props.selected ? "red" : "black";
    return (
      <TouchableOpacity onPress={this._onPress} style={{height: 50, backgroundColor: "lightyellow"}}>
        <View>
          <Text style={{ color: textColor }}>
            {this.props.title}
          </Text>
        </View>
      </TouchableOpacity>
    );
  }
}

export default class FlatListReactNative extends Component {
  state = {selected: (new Map(): Map<string, boolean>),
  data: userData,
  height: 50};

  _keyExtractor = (item, index) => item.id;

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
  />
);

updateSize = (height) => {
  this.setState({
    height
  });
}

render() {
  let newStyle = {
      height: this.state.height
    }

  return (
    <View style={{backgroundColor: "lightRed", flex: 1}} >
        <View style={{height: 50, backgroundColor: "lightgreen"}}>
        <Text> header </Text>
        </View>
        <FlatList
          data={this.state.data}
          extraData={this.state}
          keyExtractor={this._keyExtractor}
          renderItem={this._renderItem}
          inverted={true}
        />
        <TextInput defaultValue="Footer text input"
        style={[newStyle]}
        multiline={true}
        onContentSizeChange={(e) => this.updateSize(e.nativeEvent.contentSize.height)}
        >
        </TextInput>
    </View>
      );
}
}

AppRegistry.registerComponent('FlatListApp', () => FlatListReactNative)
