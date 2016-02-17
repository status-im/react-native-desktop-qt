
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
} = React;

var UbuntuActivityIndicator = requireNativeComponent('UbuntuActivityIndicator', null);

var ActivityIndicator = React.createClass({
  propTypes: {
    animating: PropTypes.bool,
  },

  render: function() {
    return (
      <UbuntuActivityIndicator
        style={{width: 20, height: 20, backgroundColor: "#ff0000"}}
        running={this.props.animating}>
      </UbuntuActivityIndicator>
    );
  }
});

module.exports = ActivityIndicator;
