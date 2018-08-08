/**
 * Copyright (C) 2016, Canonical Ltd.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * @providesModule Platform
 * @flow
 */

'use strict';

const NativeModules = require('NativeModules');

var Platform = {
  OS: 'desktop',
  get isMacOs() {
    const constants = NativeModules.PlatformConstants;
    return constants ? constants.osName === 'macos' : false;
  },
  get isWindows() {
    const constants = NativeModules.PlatformConstants;
    return constants ? constants.osName === 'windows' : false;
  },
  get isLinux() {
    const constants = NativeModules.PlatformConstants;
    return constants ? constants.osName === 'linux' : false;
  },
  get isUnix() {
    const constants = NativeModules.PlatformConstants;
    return constants ? constants.osName === 'unix' : false;
  },
  select: (obj: Object) => 'desktop' in obj ? obj.desktop : obj.default,
};

module.exports = Platform;
