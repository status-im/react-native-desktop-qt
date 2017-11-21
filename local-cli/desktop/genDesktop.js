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
// var generate = require('../generate/generate');
const { exec } = require('child_process');

function applyDesktopPlatformPatch() {
  // exec('pwd && patch --verbose -d ./node_modules/metro-bundler/src -i ../../react-native/add-ubuntu-platform.patch', (err, stdout, stderr) => {
  //   console.log(`Std output: ${stdout}`);
  //   if (err) {
  //     console.error(`exec error: ${err}`);
  //     return;
  //   }
  // });
}

function genDesktop(args, config) {
  // applyUbuntuPlatformPatch();
  // return generate([
  //   '--platform', 'ubuntu',
  //   '--project-path', process.cwd(),
  //   '--project-name', JSON.parse(
  //     fs.readFileSync('package.json', 'utf8')
  //   ).name
  // ], config);
}

module.exports = {
  name: 'desktop',
  description: 'generates an Desktop project for your app',
  func: genDesktop
};
