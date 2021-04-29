/**
 * Copyright (c) 2015-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * @providesModule TouchableNativeFeedback
 */

'use strict';

var React = require('React');
var View = require('View');
var createReactClass = require('create-react-class');

var TouchableNativeFeedback = createReactClass({
  render: function() {
    return (
      <View {...this.props} />
    );
  },
});

module.exports = TouchableNativeFeedback;
