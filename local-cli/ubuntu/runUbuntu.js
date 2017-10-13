
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
const fs = require('fs');
const isPackagerRunning = require('../util/isPackagerRunning');
const Promise = require('promise');

const parseArguments = require('./parseArguments');
const checkUbuntu = require('./checkUbuntu');
const buildUbuntu = require('./buildUbuntu');


function runUbuntu(argv, config) {
  const args = parseArguments(argv);

  return new Promise((resolve, reject) => {
    if (!checkUbuntu(args)) {
      console.log(chalk.red('Ubuntu project not found. Maybe run react-native ubuntu first?'));
    } else {
      resolve();
    }
  }).then(() => {
    return buildUbuntu(args);
  }).then(() => {
    return _runUbuntu(args, config);
  });
}

function _runUbuntu(args, config) {
  return new Promise((resolve, reject) => {
    resolve(isPackagerRunning().then(result => {
      if (result === 'running') {
        console.log(chalk.bold('JS server already running.'));
      } else if (result === 'unrecognized') {
        console.warn(chalk.yellow('JS server not recognized, continuing with build...'));
      } else {
        // result == 'not_running'
        console.log(chalk.bold('Starting JS server...'));
        startPackagerInNewWindow();
      }
      startUbuntuServerInNewWindow();
      actuallyRun(args, reject);
    }));
  });
}

function actuallyRun(args, reject) {
  process.chdir(path.join(args.root, 'ubuntu'));

  console.log(chalk.bold('Starting the app...'));
  try {
      var appArgs = [];
      if (args['live-reload'])
        appArgs.push('--live-reload');
      if (args['host'])
        appArgs.push('--host=' + args['host']);
      if (args['port'])
        appArgs.push('--port=' + args['port']);
      if (args['executor'])
        appArgs.push('--executor=' + args['executor']);
      if (args['arch'].startsWith('arm'))
        appArgs.push('--on-device');
      appArgs.push('--plugins-path=' + args['plugins-path']);
      child_process.spawnSync('./run-app.sh', appArgs,
                              {stdio: 'inherit'});
  } catch (e) {
    console.log(chalk.red('Could not start the app, see the error above.'));
    console.log(e.stdout)
    console.log(e.stderr)
    reject();
  }
}

function startPackagerInNewWindow() {
  child_process.spawn('gnome-terminal',['-e', 'npm start'],{detached: true});
}

function startUbuntuServerInNewWindow() {
  child_process.spawn('gnome-terminal', ['-e', 'node ./ubuntu/bin/ubuntu-server.js'],{detached: true});
}

module.exports = {
  name: 'run-ubuntu',
  description: 'builds and runs your app as native application',
  func: runUbuntu
};
