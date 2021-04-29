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
   let items = this.state.longList ? ["FirstButton", "SecondButton", "ThirdButton"] : ["SecondButton", "ThirdButton"];
   return (
     <View nativeID={"topView"}>
       <Button title="PressMe" onPress={this.onPress}/>
       {items.map(function(name, index) {
         return <View nativeID={name}><Button key={index} title={name}></Button></View>;
       })}
     </View>
   )
 }
}

class TestArrayReconciliationInsertFirst extends Component {
 render() {
   return (
     <ItemList />
   );
 }
}

AppRegistry.registerComponent('TestArrayReconciliationInsertFirst', () => TestArrayReconciliationInsertFirst);
