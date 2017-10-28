import React, { Component } from 'react';
import {
  AppRegistry,
  StyleSheet,
  View,
  Button
} from 'react-native';

const onButtonPress = () => {
};

export default class TestButtonSize extends Component {

   render() {
      return (
         <View nativeID={"TopView"} style = {styles.container}>
               <View nativeID={"Content"} style = {styles.modal}>
                 <Button testID={"TestButton"}
                    onPress={onButtonPress}
                    title='Long enough button'>
                 </Button>
               </View>
         </View>
      )
   }
}

const styles = StyleSheet.create ({
   container: {
      alignItems: 'center',
      backgroundColor: '#00ff00',
   },
   modal: {
      flex: 1,
      alignItems: 'center',
      backgroundColor: '#ff0000',
   }
})

AppRegistry.registerComponent('TestButtonSize', () => TestButtonSize)
