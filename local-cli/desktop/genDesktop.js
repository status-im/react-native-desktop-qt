/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

'use strict';

const fs = require('fs');
var generate = require('../generate/generate');
const { exec } = require('child_process');

function applyDesktopPlatformPatch() {
  exec('git apply --reverse ./node_modules/react-native/patches/metro-config+0.48.5.patch');
  exec('git apply ./node_modules/react-native/patches/metro-config+0.48.5.patch', (err, stdout, stderr) => {
     console.log(`Std output: ${stdout}`);
     if (err) {
       console.error(`exec error: ${err}`);
       return;
     }
   });
}

function genDesktop(args, config) {
  return applyDesktopPlatformPatch();
}

module.exports = {
  name: 'desktop',
  description: 'generates a desktop project for your app',
  func: genDesktop
};
