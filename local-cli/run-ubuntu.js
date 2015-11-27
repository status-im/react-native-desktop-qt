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
  return fs.existsSync('ubuntu/CMakeList.txt') && 
         process.platform.startsWith('linux');
}

function buildAndRun() {
  process.chdir('ubuntu');
  try {
    var cmakeArgs = ['installDebug'].concat(process.argv.slice(3));
    console.log(chalk.bold('Building and installing the app on the device (cd ubuntu && ' + cmd + ' ' + cmakeArgs.join(' ') + ')...'));
    child_process.execFileSync(cmd, cmakeArgs, {
      stdio: [process.stdin, process.stdout, process.stderr]
    });
  } catch (e) {
    console.log(chalk.red('Could not install the app on the device, see the error above.'));
    // stderr is automatically piped from the gradle process, so the user should see the error
    // already, there is no need to do console.log(e.stderr)
    return;
  }
  try {
    var packageName = fs.readFileSync('app/src/main/AndroidManifest.xml', 'utf8').match(/package="(.+?)"/)[1];
    var adbPath = process.env.ANDROID_HOME ? process.env.ANDROID_HOME + '/platform-tools/adb' : 'adb';
    var adbArgs = ['shell', 'am', 'start', '-n', packageName + '/.MainActivity'];
    console.log(chalk.bold('Starting the app (' + adbPath + ' ' + adbArgs.join(' ') + ')...'));
    child_process.spawnSync(adbPath, adbArgs, {
      stdio: [process.stdin, process.stdout, process.stderr]
    });
  } catch (e) {
    console.log(chalk.red('adb invocation failed. Do you have adb in your PATH?'));
    // stderr is automatically piped from the adb process, so the user should see the error already,
    // there is no need to do console.log(e.stderr)
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
