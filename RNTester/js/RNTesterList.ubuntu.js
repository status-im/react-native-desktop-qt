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
    key: 'ImageExample',
    module: require('./ImageExample'),
    supportsTVOS: true,
  },
];

const APIExamples: Array<RNTesterExample> = [

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
