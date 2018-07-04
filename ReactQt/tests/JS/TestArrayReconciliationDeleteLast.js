import React, { Component } from 'react';
import {
 AppRegistry,
 Alert,
 Text,
 TouchableHighlight,
 View,
 TextInput,
 Button
} from 'react-native';



class ItemList extends Component {
 constructor(props) {
   super(props);
   this.state = {longList: true};
   this.onPress = this.onPress.bind(this);
 }

 onPress() {
   this.setState({longList: !this.state.longList});
 }

 onButtonPress() {
  this.setState({longList: !this.state.longList});
 }

 render() {
   let items = this.state.longList ? ["FirstButton", "SecondButton", "ThirdButton"] : ["1stButton", "2ndButton"];
   return (
     <View onPress={this.onPress} nativeID={"topView"}>
       {items.map(function(name, index) {
         return <Button key={name} title={name} > </Button>;
       })}
     </View>
   )
 }
}

class TestArrayReconciliationDeleteLast extends Component {
 render() {
   return (
     <ItemList />
   );
 }
}

AppRegistry.registerComponent('TestArrayReconciliationDeleteLast', () => TestArrayReconciliationDeleteLast);