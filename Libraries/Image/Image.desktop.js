/**
 * Copyright (C) 2016, Canonical Ltd.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * @providesModule Image
 */

'use strict';

var React = require('React');
const PropTypes = require('prop-types');

var NativeModules = require('NativeModules');
var ImageResizeMode = require('ImageResizeMode');
var ImageStylePropTypes = require('ImageStylePropTypes');
var StyleSheetPropType = require('StyleSheetPropType');
var NativeMethodsMixin = require('NativeMethodsMixin');
var ReactNativeViewAttributes = require('ReactNativeViewAttributes');
const ImageSourcePropType = require('ImageSourcePropType');

var requireNativeComponent = require('requireNativeComponent');
var resolveAssetSource = require('resolveAssetSource');
const createReactClass = require('create-react-class');

var {
  ImageLoader,
} = NativeModules;

var Image = createReactClass({
  propTypes: {
    style: StyleSheetPropType(ImageStylePropTypes),
    source: PropTypes.oneOfType([
      PropTypes.shape({
        uri: PropTypes.string,
      }),
      PropTypes.number,
    ]),
    onLoadStart: PropTypes.func,
    onProgress: PropTypes.func,
    onError: PropTypes.func,
    onLoad: PropTypes.func,
    onLoadEnd: PropTypes.func,
    testID: PropTypes.string,
    blurRadius: PropTypes.number,
    onLayout: PropTypes.func,
  },

  statics: {
    resizeMode: ImageResizeMode,

    getSize(
      url: string,
      success: (width: number, height: number) => void,
      failure?: (error: any) => void,
    ) {
      return ImageLoader.getSize(url)
        .then(function(sizes) {
          success(sizes.width, sizes.height);
        })
        .catch(failure || function() {
          console.warn('Failed to get size for image: ' + url);
        });
    },


    prefetch(url: string) {
      return ImageLoader.prefetchImage(url);
    },
  },

  mixins: [NativeMethodsMixin],

  viewConfig: {
    uiViewClassName: 'UIView',
    validAttributes: ReactNativeViewAttributes.UIView
  },

  render: function() {
    var source = resolveAssetSource(this.props.source) || {};

    return (
      <RCTImageView
        {...this.props}
        source={source}
      />
    );
  }
});

var RCTImageView = requireNativeComponent('RCTImageView', Image);

module.exports = Image;
