/**
 * @providesModule Image
 */
'use strict';

var React = require('React');
var PropTypes = React.PropTypes;

var requireNativeComponent = require('requireNativeComponent');
var resolveAssetSource = require('resolveAssetSource');

var Image = React.createClass({
  propTypes: {
    source: PropTypes.shape({uri: PropTypes.string}),
  },

  render: function() {
    var source = resolveAssetSource(this.props.source) || {};
    var {width, height} = source;

    return (
      <RCTImageView
        source={this.props.source}
        style={{width, height}}
        >
      </RCTImageView>
    );
  }
});

var RCTImageView = requireNativeComponent('RCTImageView', Image);

module.exports = Image;
