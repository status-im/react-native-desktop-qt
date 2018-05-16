
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
const checkDesktop = require('./checkDesktop');
const buildDesktop = require('./build');


function runDesktop(argv, config) {
  const args = parseArguments(argv);

  return new Promise((resolve, reject) => {
    if (!checkDesktop(args)) {
      console.log(chalk.red('Desktop project not found. Maybe run react-native desktop first?'));
    } else {
      resolve();
    }
  }).then(() => {
    return buildDesktop(args);
  }).then(() => {
    return _runDesktop(args, config);
  });
}

function _runDesktop(args, config) {
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
  process.chdir(path.join(args.root, 'desktop'));

  console.log(chalk.bold('Starting the app...'));
  try {
      var appArgs = [];
      if (args['live-reload'])
        appArgs.push('--live-reload');
      if (args['host'])
        appArgs.push('--host=' + args['host']);
      if (args['port'])
        appArgs.push('--port=' + args['port']);
      if (args['executor']) {
        appArgs.push('--executor=' + args['executor']);
      } else {
        appArgs.push('--executor=RemoteServerConnection');
      }
      if (args['arch'].startsWith('arm'))
        appArgs.push('--on-device');
      appArgs.push('--plugins-path=' + args['plugins-path']);
      child_process.spawnSync(process.platform === "win32" ? 'run-app.bat' : './run-app.sh', appArgs,
                              {stdio: 'inherit'});
  } catch (e) {
    console.log(chalk.red('Could not start the app, see the error above.'));
    console.log(e.stdout)
    console.log(e.stderr)
    reject();
  }
}

function startPackagerInNewWindow() {
  if (process.platform === "win32") {
    child_process.spawn('cmd', ['/c', 'npm start'],{detached: true});
  } else {
    child_process.spawn('gnome-terminal',['-e', 'npm start'],{detached: true});
  }
}

function startUbuntuServerInNewWindow() {
  if (process.platform === "win32") {
    child_process.spawn('cmd', ['/c', 'node ./desktop/bin/ubuntu-server.js'],{detached: true});
  } else {
    child_process.spawn('gnome-terminal', ['-e', 'node ./desktop/bin/ubuntu-server.js'],{detached: true});
  }
}

module.exports = {
  name: 'run-desktop',
  description: 'builds and runs your app as native application',
  func: runDesktop
};
