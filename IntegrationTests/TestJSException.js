/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * @flow
 * @providesModule TestJSException
 */
'use strict';

var React = require('react');
var createReactClass = require('create-react-class');
var ReactNative = require('react-native');

var {
  AppRegistry,
  Text,
  View,
} = ReactNative;

var TestJSException = createReactClass({
  displayName: 'TestJSException',

  componentDidMount() {
    throw new Error('Exception on componentDidMount. File TestJSException.js');
  },

  render() {
    return (
      <View>
        <Text>
          Handling of JS exception
        </Text>
      </View>
    );
  }
});

TestJSException.displayName = 'TestJSException';
AppRegistry.registerComponent('TestJSException', () => TestJSException);

module.exports = TestJSException;
