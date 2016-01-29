/**
 * @providesModule Image
 */
'use strict';

var React = require('React');
var PropTypes = React.PropTypes;

var ImageResizeMode = require('ImageResizeMode');

var requireNativeComponent = require('requireNativeComponent');
var resolveAssetSource = require('resolveAssetSource');

var Image = React.createClass({
  propTypes: {
    source: PropTypes.shape({uri: PropTypes.string}),
    resizeMode: PropTypes.oneOf(['cover', 'contain', 'stretch']),
  },

  statics: {
    resizeMode: ImageResizeMode,
  },

  render: function() {
    var source = resolveAssetSource(this.props.source) || {};

    return (
      <RCTImageView
        source={source.uri}
        style={this.props.style}
        >
      </RCTImageView>
    );
  }
});

var RCTImageView = requireNativeComponent('RCTImageView', Image);

module.exports = Image;
