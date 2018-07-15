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
   this.state = {longList: true,
                 itemsList: ["FirstButton", "SecondButton", "ThirdButton", "FourthButton"] };
   this.onPress = this.onPress.bind(this);
 }

 onPress() {
   let removedItems = this.state.itemsList.splice(0, 1);
   this.state.itemsList.push(removedItems[0]);
   this.setState({longList: !this.state.longList,
                  itemsList: this.state.itemsList});
 }

 render() {
   return (
     <View onPress={this.onPress} nativeID={"topView"}>
       {this.state.itemsList.map(function(name, index) {
         return <Button key={name} title={name} > </Button>;
       })}
     </View>
   )
 }
}

class TestArrayReconciliationItemMove extends Component {
 render() {
   return (
     <ItemList />
   );
 }
}

AppRegistry.registerComponent('TestArrayReconciliationItemMove', () => TestArrayReconciliationItemMove);
