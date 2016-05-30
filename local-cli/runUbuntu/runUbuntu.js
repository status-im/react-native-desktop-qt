
/**
 * Copyright (C) 2016, Canonical Ltd.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

'use strict';

const path = require('path');
const chalk = require('chalk');
const child_process = require('child_process');
const fs = require('fs');
const parseCommandLine = require('../util/parseCommandLine');
const isPackagerRunning = require('../util/isPackagerRunning');
const Promise = require('promise');

function runUbuntu(argv, config) {
  return new Promise((resolve, reject) => {
    _runUbuntu(argv, config, resolve, reject);
  });
}

function _runUbuntu(argv, config, resolve, reject) {
  const args = parseCommandLine([{
    command: 'root',
    type: 'string',
    description: 'Override the root directory for the ubuntu build (which contains the ubuntu directory)',
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
    command: 'arch',
    type: 'string',
    description: 'Set build architecture',
    default: process.arch
  }], argv);
  args.root = args.root || '';

  if (!checkUbuntu(args)) {
    console.log(chalk.red('Ubuntu project not found. Maybe run react-native ubuntu first?'));
    return;
  }

  resolve(isPackagerRunning().then(result => {
    if (result === 'running') {
      console.log(chalk.bold('JS server already running.'));
    } else if (result === 'unrecognized') {
      console.warn(chalk.yellow('JS server not recognized, continuing with build...'));
    } else {
      // result == 'not_running'
      console.log(chalk.bold('Starting JS server...'));
      startServerInNewWindow();
    }
    buildAndRun(args, reject);
  }));
}

function checkUbuntu(args) {
  return fs.existsSync(path.join(args.root, 'ubuntu/CMakeLists.txt')) &&
         process.platform.startsWith('linux');
}

function buildAndRun(args, reject) {
  process.chdir(path.join(args.root, 'ubuntu'));

  console.log(chalk.bold('Building the app...'));
  try {
    if (args['arch'].startsWith('arm')) {
      child_process.spawnSync('click',
                              'chroot -a armhf -f ubuntu-sdk-15.04 -n click run ./build.sh'.split(' '),
                              {stdio: 'inherit'})
    } else {
      child_process.spawnSync('./build.sh',
                              [],
                              {stdio: 'inherit'});
    }
  } catch (e) {
      console.log(chalk.red('Could not build the app, see the error above.'));
      console.log(e.stdout)
      console.log(e.stderr)
      return;
  }

  console.log(chalk.bold('Starting the app...'));
  try {
      var appArgs = [];
      if (args['live-reload'])
        appArgs.push('--live-reload');
      if (args['host'])
        appArgs.push('--host=' + args['host']);
      if (args['port'])
        appArgs.push('--port=' + args['port']);
      if (args['arch'].startsWith('arm'))
        appArgs.push('--on-device');
      child_process.spawnSync('./run-app.sh', appArgs,
                              {stdio: 'inherit'});
  } catch (e) {
    console.log(chalk.red('Could not start the app, see the error above.'));
    console.log(e.stdout)
    console.log(e.stderr)
    reject();
  }
}

function startServerInNewWindow() {
  const packagerPath = path.resolve(__dirname, '..', '..', 'packager', 'packager.sh');
  child_process.spawn('gnome-terminal',['-e', packagerPath],{detached: true});
}

module.exports = runUbuntu;
