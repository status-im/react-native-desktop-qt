
/*
 * @providesModule ActivityIndicator
 * @flow
 */


'use strict';

var React = require('react-native');
var PropTypes = require('ReactPropTypes');
var requireNativeComponent = require('requireNativeComponent');

var {
  View,
  StyleSheet,
} = React;


var UbuntuActivityIndicator = requireNativeComponent('UbuntuActivityIndicator', null);

var ActivityIndicator = React.createClass({
  propTypes: {
    animating: PropTypes.bool,
    size: PropTypes.oneOf([
      'small',
      'large',
    ]),
  },

  getDefaultProps: function() {
    return {
      animating: true,
      size: 'small',
    };
  },

  render: function() {
    var { animating, size } = this.props;
    return (
      <UbuntuActivityIndicator
        style={size === 'large' ? styles.sizeLarge : styles.sizeSmall}
        running={animating}>
      </UbuntuActivityIndicator>
    );
  }
});

var styles = StyleSheet.create({
  sizeSmall: {
    width: 20,
    height: 20,
  },
  sizeLarge: {
    width: 36,
    height: 36,
  }
});

module.exports = ActivityIndicator;
