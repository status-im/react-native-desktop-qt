
/**
 * Copyright (C) 2016, Canonical Ltd.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * Author: Justin McPherson <justin.mcpherson@canonical.com>
 *
 */

'use strict';

const path = require('path');
const chalk = require('chalk');
const child_process = require('child_process');
const parseArguments = require('./parseArguments');
const build = require('./build');


function buildDesktop(argv, config) {
  const args = parseArguments(argv);
  build(args);
}

module.exports = {
  name: 'build-desktop',
  description: 'builds your app as native application',
  func: buildDesktop
};
