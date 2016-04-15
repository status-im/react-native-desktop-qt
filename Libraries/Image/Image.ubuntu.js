/**
 * @providesModule Image
 */
'use strict';

var React = require('React');
var PropTypes = React.PropTypes;

var ImageResizeMode = require('ImageResizeMode');
var ImageStylePropTypes = require('ImageStylePropTypes');
var StyleSheetPropType = require('StyleSheetPropType');


var requireNativeComponent = require('requireNativeComponent');
var resolveAssetSource = require('resolveAssetSource');

var Image = React.createClass({
  propTypes: {
    style: StyleSheetPropType(ImageStylePropTypes),
    source: PropTypes.shape({uri: PropTypes.string}),
  },

  statics: {
    resizeMode: ImageResizeMode,
  },

  render: function() {
    var source = resolveAssetSource(this.props.source) || {};

    return (
      <RCTImageView
        { ...this.props }
        source={source.uri}
        >
      </RCTImageView>
    );
  }
});

var RCTImageView = requireNativeComponent('RCTImageView', Image);

module.exports = Image;
