/**
 * Copyright 2015-present Canonical Ltd.
 * 
 * Adapted from run-android.js
 * BSD-License
 */

'use strict';

var chalk = require('chalk');
var child_process = require('child_process');
var fs = require('fs');
var http = require('http');
var runPackager = require('./run-packager.js');

function checkUbuntu() {
  return fs.existsSync('ubuntu/CMakeLists.txt') && 
         process.platform.startsWith('linux');
}

function buildAndRun() {
  process.chdir('ubuntu');

  console.log(chalk.bold('Building the app...'));
  try {
    var cmd = 'cmake';
    var cmdArgs = '.';
    child_process.execFileSync(cmd, cmdArgs, {
      stdio: [process.stdin, process.stdout, process.stderr]
    });
    cmd = 'make'; cmdArgs = '';
    child_process.execFileSync(cmd, cmdArgs, {
      stdio: [process.stdin, process.stdout, process.stderr]
    });
  } catch (e) {
    console.log(chalk.red('Could not build the app, see the error above.'));
    console.log(e.stdout)
    console.log(e.stderr)
  }

  console.log(chalk.bold('Starting the app (desktop mode)...'));
  try {
    var cmd = 'run-app.sh';
    var cmdArgs = '';
    child_process.execFileSync(cmd, cmdArgs, {
      stdio: [process.stdin, process.stdout, process.stderr]
    });
  } catch (e) {
    console.log(chalk.red('Failed to start the app, see the logs'));
    return;
  }
}

module.exports = function() {
  if (!checkUbuntu()) {
    console.log(chalk.red('Ubuntu project not found. Maybe run react-native ubuntu first?'));
    return;
  }
  // is packager running?
  var statusReq = http.get('http://localhost:8081/status', function(res) {
    var response = '';
    res.on('data', function(chunk) {
      response += chunk;
    });
    res.on('end', function() {
      if (response === 'packager-status:running') {
        console.log(chalk.bold('JS server already running.'));
      } else {
        console.log(chalk.yellow('[warn] JS server not recognized, continuing with build...'));
      }
      buildAndRun();
      // make sure we don't wait around for the packager process
      process.exit();
    });
  });
  statusReq.on('error', function() {
    // start packager first so it warms up
    console.log(chalk.bold('Starting JS server...'));
    runPackager(true);
    buildAndRun();
  });
};
