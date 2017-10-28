
import React, { Component } from 'react';
import {
  AppRegistry,
  StyleSheet,
  Text,
  View,
  Modal,
  Button
} from 'react-native';

const onButtonPress = () => {
};

export default class ModalReactNative extends Component {

   render() {
      return (
        <Modal animationType = {"slide"} transparent = {false}
           onShow = {()=>{console.log("onShow()")}}
           visible = {true}>
           <View nativeID="ModalInner" style = {styles.modal}>
              <Text style = {styles.text}>Modal is open!</Text>
              <Button title='Close Modal'
                onPress={onButtonPress}/>
           </View>
        </Modal>
      )
   }
}

const styles = StyleSheet.create ({
   modal: {
      flex: 1,
      alignItems: 'center',
      backgroundColor: '#00ff00',
      padding: 100
   },
   text: {
      color: '#3f2949',
      marginTop: 10
   }
})

AppRegistry.registerComponent('TestModalProps', () => ModalReactNative);
