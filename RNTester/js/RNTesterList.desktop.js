/**
 * Copyright (c) 2015-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * @flow
 * @providesModule RNTesterList
 */
'use strict';

export type RNTesterExample = {
  key: string,
  module: Object,
};

const ComponentExamples: Array<RNTesterExample> = [
  {
    key: 'ActivityIndicatorExample',
    module: require('./ActivityIndicatorExample'),
  },
  {
    key: 'ImageExample',
    module: require('./ImageExample'),
  },
  {
    key: 'TextExample',
    module: require('./TextExample'),
  },
  {
    key: 'ButtonExample',
    module: require('./ButtonExample'),
  },
  {
    key: 'LayoutExample',
    module: require('./LayoutExample'),
  },
  {
    key: 'SliderExample',
    module: require('./SliderExample'),
  },
  {
    key: 'TextInputExample',
    module: require('./TextInputExample'),
  },
  {
    key: 'TouchableExample',
    module: require('./TouchableExample'),
  },
  {
    key: 'PickerExample',
    module: require('./PickerExample'),
  },
  {
    key: 'WebSocketExample',
    module: require('./WebSocketExample'),
  },
  {
    key: 'WebViewExample',
    module: require('./WebViewExample'),
  },
  {
    key: 'ViewExample',
    module: require('./ViewExample'),
  },
  {
    key: 'SwitchExample',
    module: require('./SwitchExample'),
  }
];

const APIExamples: Array<RNTesterExample> = [
    {
      key: 'ClipboardExample',
      module: require('./ClipboardExample'),
    },
    {
      key: 'AlertExample',
      module: require('./AlertExample').AlertExample,
    },
];

const Modules = {};

APIExamples.concat(ComponentExamples).forEach(Example => {
  Modules[Example.key] = Example.module;
});

const RNTesterList = {
  APIExamples,
  ComponentExamples,
  Modules,
};

module.exports = RNTesterList;
