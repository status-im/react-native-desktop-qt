/**
 * @providesModule ActivityIndicatorIOS
 * @flow
 */
'use strict';

var NativeMethodsMixin = require('NativeMethodsMixin');
var PropTypes = require('ReactPropTypes');
var React = require('React');
var StyleSheet = require('StyleSheet');
var View = require('View');

var requireNativeComponent = require('requireNativeComponent');

var GRAY = '#999999';

type DefaultProps = {
  running: boolean;
};

var ActivityIndicatorIOS = React.createClass({
  mixins: [NativeMethodsMixin],

  propTypes: {
    running: PropTypes.bool,
    /**
     * Invoked on mount and layout changes with
     *
     *   {nativeEvent: { layout: {x, y, width, height}}}.
     */
    onLayout: PropTypes.func,
  },

  getDefaultProps: function(): DefaultProps {
    return {
      running: true,
    };
  },

  render: function() {
    var {onLayout, style, ...props} = this.props;
    var sizeStyle = (this.props.size === 'large') ? styles.sizeLarge : styles.sizeSmall;
    return (
      <View
        onLayout={onLayout}
        style={[styles.container, sizeStyle, style]}>
        <RCTActivityIndicatorView {...props} />
      </View>
    );
  }
});

var styles = StyleSheet.create({
  container: {
    alignItems: 'center',
    justifyContent: 'center',
  },
  sizeSmall: {
    width: 20,
    height: 20,
  },
  sizeLarge: {
    width: 36,
    height: 36,
  }
});

var RCTActivityIndicatorView = requireNativeComponent(
  'RCTActivityIndicatorView',
  ActivityIndicatorIOS,
  {nativeOnly: {activityIndicatorViewStyle: true}},
);

module.exports = ActivityIndicatorIOS;
