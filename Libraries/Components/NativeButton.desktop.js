/**
 * Copyright (c) 2015-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 *
 * @format
 * @flow
 */
'use strict';

const ColorPropType = require('ColorPropType');
const NativeMethodsMixin = require('NativeMethodsMixin');
const Platform = require('Platform');
const React = require('React');
const PropTypes = require('prop-types');
const View = require('View');
const ViewPropTypes = require('ViewPropTypes');

const createReactClass = require('create-react-class');
const requireNativeComponent = require('requireNativeComponent');

/**
 * Displays a native button
 */
const NativeButton = createReactClass({
  displayName: 'Button',
  mixins: [NativeMethodsMixin],

  propTypes: {
    title: PropTypes.string.isRequired,
    accessibilityLabel: PropTypes.string,
    color: ColorPropType,
    disabled: PropTypes.bool,
    onPress: PropTypes.func.isRequired,
    testID: PropTypes.string,
  },


  render() {
    return (
        <RCTButton
          {...this.props}
        />
    );
  }
});

var RCTButton = requireNativeComponent('RCTButtonView', NativeButton);

module.exports = NativeButton;
