/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 * @flow
 */

import React, { Component } from 'react';
import {
  AppRegistry,
  StyleSheet,
  Text,
  ScrollView,
  Image
} from 'react-native';

export default class ScrollVieReactNative extends Component {
  render() {
    return (
        <ScrollView>
            <Text style={{fontSize:96}}>Scroll me plz</Text>
            <Image
                source={{uri: 'https://facebook.github.io/react-native/img/favicon.png'}}
                style={{width: 100, height: 100}}
            />
            <Image
                source={{uri: 'https://facebook.github.io/react-native/img/favicon.png'}}
                style={{width: 100, height: 100}}
            />
            <Image
                source={{uri: 'https://facebook.github.io/react-native/img/favicon.png'}}
                style={{width: 100, height: 100}}
            />
            <Image
                source={{uri: 'https://facebook.github.io/react-native/img/favicon.png'}}
                style={{width: 100, height: 100}}
            />
            <Image
                source={{uri: 'https://facebook.github.io/react-native/img/favicon.png'}}
                style={{width: 100, height: 100}}
            />
            <Text style={{fontSize:96}}>If you like</Text>
            <Image
                source={{uri: 'https://facebook.github.io/react-native/img/favicon.png'}}
                style={{width: 100, height: 100}}
            />
            <Image
                source={{uri: 'https://facebook.github.io/react-native/img/favicon.png'}}
                style={{width: 100, height: 100}}
            />
            <Image
                source={{uri: 'https://facebook.github.io/react-native/img/favicon.png'}}
                style={{width: 100, height: 100}}
            />
            <Image
                source={{uri: 'https://facebook.github.io/react-native/img/favicon.png'}}
                style={{width: 100, height: 100}}
            />
            <Image
                source={{uri: 'https://facebook.github.io/react-native/img/favicon.png'}}
                style={{width: 100, height: 100}}
            />
            <Text style={{fontSize:96}}>Scrolling down</Text>
            <Image
                source={{uri: 'https://facebook.github.io/react-native/img/favicon.png'}}
                style={{width: 100, height: 100}}
            />
            <Image
                source={{uri: 'https://facebook.github.io/react-native/img/favicon.png'}}
                style={{width: 100, height: 100}}
            />
            <Image
                source={{uri: 'https://facebook.github.io/react-native/img/favicon.png'}}
                style={{width: 100, height: 100}}
            />
            <Image
                source={{uri: 'https://facebook.github.io/react-native/img/favicon.png'}}
                style={{width: 100, height: 100}}
            />
            <Image
                source={{uri: 'https://facebook.github.io/react-native/img/favicon.png'}}
                style={{width: 100, height: 100}}
            />
            <Text style={{fontSize:96}}>What's the best</Text>
            <Image
                source={{uri: 'https://facebook.github.io/react-native/img/favicon.png'}}
                style={{width: 100, height: 100}}
            />
            <Image
                source={{uri: 'https://facebook.github.io/react-native/img/favicon.png'}}
                style={{width: 100, height: 100}}
            />
            <Image
                source={{uri: 'https://facebook.github.io/react-native/img/favicon.png'}}
                style={{width: 100, height: 100}}
            />
            <Image
                source={{uri: 'https://facebook.github.io/react-native/img/favicon.png'}}
                style={{width: 100, height: 100}}
            />
            <Image
                source={{uri: 'https://facebook.github.io/react-native/img/favicon.png'}}
                style={{width: 100, height: 100}}
            />
            <Text style={{fontSize:80}}>React Native</Text>
          </ScrollView>
    );
  }
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
    backgroundColor: '#F5FCFF',
  },
  welcome: {
    fontSize: 20,
    textAlign: 'center',
    margin: 10,
  },
  instructions: {
    textAlign: 'center',
    color: '#333333',
    marginBottom: 5,
  },
});

AppRegistry.registerComponent('ScrollViewApp', () => ScrollVieReactNative);
