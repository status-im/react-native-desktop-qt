
import React, { Component } from 'react';
import {
  AppRegistry,
  StyleSheet,
  Text,
  View,
  Modal,
  Button
} from 'react-native';

export default class ModalReactNative extends Component {

    state = {
      modalVisible: false,
   }
   toggleModal(visible) {
      this.setState({ modalVisible: visible });
   }
   render() {
      return (
         <View style = {styles.container}>
            <Modal animationType = {"slide"} transparent = {false}
               visible = {this.state.modalVisible}
               onShow = {() => {
                  console.log("onShow()")}}>
               <View nativeID="ModalInner" style = {styles.modal}>
                  <Text style = {styles.text}>Modal is open!</Text>

                  <Button onPress = {() => {
                     this.toggleModal(!this.state.modalVisible)}}
                     title='Close Modal'>
                  </Button>
               </View>
            </Modal>
            <Text style = {styles.text}>Modal will hide this</Text>
            <Button onPress = {() => {this.toggleModal(true)}}
            title='Open Modal'>
            </Button>
         </View>
      )
   }
}

const styles = StyleSheet.create ({
   container: {
      alignItems: 'center',
      backgroundColor: '#ede3f2',
      padding: 100
   },
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

AppRegistry.registerComponent('ModalApp', () => ModalReactNative)
