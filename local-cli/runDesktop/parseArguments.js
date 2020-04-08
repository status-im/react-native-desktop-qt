
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

const parseCommandLine = require('../util/parseCommandLine');

function parseArguments(argv) {
  const args = parseCommandLine([{
    command: 'root',
    type: 'string',
    description: 'Override the root directory for the desktop build (which contains the desktop directory)',
  }, {
    command: 'live-reload',
    description: 'Start with live reloading enabled',
    default: false
  }, {
    command: 'host',
    type: 'string',
    description: 'Set packager host'
  }, {
    command: 'port',
    type: 'string',
    description: 'Set packager port'
  }, {
    command: 'executor',
    type: 'string',
    description: 'Name of executor to use'
  }, {
    command: 'arch',
    type: 'string',
    description: 'Set build architecture',
    default: process.arch
  }, {
    command: 'plugins-path',
    type: 'string',
    description: 'Path to node modules',
    default: process.cwd() + "/desktop/plugins"
  }, {
    command: 'click',
    description: 'Build a click package',
    default: false
  }, {
    command: 'snap',
    description: 'Build a click package',
    default: false
  }], argv);
  args.root = args.root || '';

  return args;
}

module.exports = parseArguments;
