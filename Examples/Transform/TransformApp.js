
// Display views and an image with various transforms applied

import React, { Component } from 'react';
import {
  AppRegistry,
  View,
  Text,
  Image,
  Easing, 
  StyleSheet
} from 'react-native';
import PropTypes from 'prop-types';

class Tricolor extends Component {


    static propTypes = {
        content: PropTypes.string.isRequired,
        containerStyles: PropTypes.oneOfType([
            PropTypes.array,
            PropTypes.number,
            PropTypes.shape({}),
        ]),
    }


render() {
    const { containerStyles, content } = this.props;
    return (
        <View style={[styles.tricolor, containerStyles]}>
            <View style={[styles.flexstripestyle, {backgroundColor:'rgba(255, 0, 0, 1.0)'}]}>
                <Text> t-{content} </Text> 
            </View>
            <View style={[styles.flexstripestyle, {backgroundColor:'rgba(0, 255, 0, 1.0)'}]}>
                <Text> t-Second </Text> 
            </View>
            <View style={[styles.flexstripestyle, {backgroundColor:'rgba(0, 0, 255, 1.0)'}]}>   
                <Text> t-Third </Text> 
            </View>
        </View>
    );
   }
};


export default class App extends Component {

  render() {

    const towerWidth = 12;
    const borderRadiusVal = towerWidth/3;
    const bridgeHeight = 35;
    const source =  {uri: 'https://www.google.com/images/branding/googlelogo/2x/googlelogo_color_120x44dp.png'};

    return (
      <View style={styles.container}>
        <View style={styles.logo}>
          <Tricolor content={'Hello'} />
          <Tricolor containerStyles={styles.upShift} content={'Shifted Y'} />
          <Tricolor containerStyles={styles.rotatez} content={'Rotated'} />
          <Tricolor containerStyles={styles.scaley} content={'Scale Y'} />
          <Tricolor containerStyles={styles.distance} content={'Perspective'} />
           <View style={{width:40}}/>
       </View> 
        <View style={{height:140}}/>
        <View style={styles.logo}>
          <View style={[styles.barstyle, {backgroundColor:'rgba(255, 100, 0, 0.8)'}]} />
          <View style={[styles.barstyle, styles.upShift, {backgroundColor:'rgba(255, 100, 0, 0.8)'}]} />
          <View style={[styles.barstyle, styles.rotatey, {backgroundColor:'rgba(0, 255, 0, 0.8)'}]} />
          <View style={[styles.barstyle, styles.rotatez, {backgroundColor:'rgba(0, 0, 255, 0.8)'}]} >
            <Text> rotated Z </Text> 
          </View>
          <View style={[styles.barstyle, styles.scaley, {backgroundColor:'rgba(0, 255, 0, 0.8)'}]} />
          <View style={[styles.slabstyle, styles.distance, {backgroundColor:'rgba(255, 255, 255, 0.8)'}]} >
            <Text> vanish </Text> 
          </View> 
          <View style={{width:40}}/>
        </View>
        <View style={{height:100}}/>
        <View style={styles.logo}>
          <Image source={source} style={[styles.imagestyle, {backgroundColor:'rgba(255, 100, 0, 0.8)'}]} />
          <Image source={source} style={[styles.imagestyle, styles.upShift, {backgroundColor:'rgba(255, 100, 0, 0.8)'}]} />
          <Image source={source} style={[styles.imagestyle, styles.rotatez, {backgroundColor:'rgba(0, 0, 255, 0.8)'}]} />
          <Image source={source} style={[styles.imagestyle, styles.scaley, {backgroundColor:'rgba(0, 255, 0, 0.8)'}]} />
          <Image source={source} style={[styles.imagestyle, styles.distance, {backgroundColor:'rgba(255, 255, 255, 0.8)'}]} />
          <View style={{width:40}}/>
        </View>
     </View> 
    );
  }
};

const styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: 'center',
    backgroundColor: 'rgba(255, 1, 255, 0.8)',
    flexDirection: 'column',
  },
  logo: { 
    flexDirection: 'row',
    justifyContent: 'space-between',
    alignItems: 'flex-end',
    height: 60
  },
  barstyle: {
    margin: 10,
    marginBottom: 35, 
    width: 22,  // towerwidth
    height: 66, 
    borderRadius: 4  //borderRadiusVal,
  },
  slabstyle: {
    margin: 10,
    marginBottom: 35, 
    width: 132,  // towerwidth
    height: 56, 
  },
  imagestyle: {
    width: 120,
    height: 50,
  },
  tricolor: { 
    flex: 1,
    flexDirection: 'column',
    alignItems: 'stretch',
    backgroundColor: 'rgba(255, 255, 255, 1.0)'
  },
  stripestyle: {
    width: 132, 
    height: 56, 
  },
  flexstripestyle: {
    width: 132,  
  },
  rightShift: {
    transform: [{ translateX: 20 }],
  },
  upShift: {
    transform: [{ translateY: 20 }],
  },
 rotatex: {
     transform: [{ rotateX: '45deg' }],
   },
   rotatey: {
     transform: [{ rotateY: '45deg' }],
   },
   rotatez: {
     transform: [{ rotate: '45deg' }],
   },
   scaley: {
     transform: [{ scaleY: 2.50 }],
   },
   distance: {
    transform: [
     { perspective: 250 },
     { translateX: 20 },
     { rotateY: '20deg'},
     { rotateX: '40deg'},
    ],
    },
});

AppRegistry.registerComponent('TransformApp', () => App);


