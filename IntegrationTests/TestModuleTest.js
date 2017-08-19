/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * @flow
 * @providesModule TestModuleTest
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
var { TestModule  } = ReactNative.NativeModules;

var TestModuleTest = createReactClass({
  displayName: 'TestModuleTest',

  componentDidMount() {
    TestModule.markTestCompleted();
  },

  render() {
    return (
      <View>
        <Text>
          Testing native TestModule
        </Text>
      </View>
    );
  }
});

TestModuleTest.displayName = 'TestModuleTest';
AppRegistry.registerComponent('TestModuleTest', () => TestModuleTest);

module.exports = TestModuleTest;
