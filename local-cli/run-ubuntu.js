/**
 * Copyright 2015-present Canonical Ltd.
 * 
 * Adapted from run-android.js
 * BSD-License
 */

'use strict';

var path = require('path');
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
        child_process.spawnSync('sh',
                                ['-c', "cmake . && make"],
                                {stdio: 'inherit'});
    } catch (e) {
        console.log(chalk.red('Could not build the app, see the error above.'));
        console.log(e.stdout)
        console.log(e.stderr)
        return;
    }
    
    console.log(chalk.bold('Starting the app...'));

    try {
        child_process.spawnSync('./run-app.sh', [],
                                {stdio: 'inherit'});
    } catch (e) {
        console.log(chalk.red('Could not start the app, see the error above.'));
        console.log(e.stdout)
        console.log(e.stderr)
        return;
    }
}

function startServer() {

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
        console.log(chalk.bold('Packager already running.'));
      } else {
        console.log(chalk.yellow('[warn] Packager not recognized, continuing with build...'));
      }
      buildAndRun();
      // make sure we don't wait around for the packager process
      process.exit();
    });
  });
  statusReq.on('error', function() {
    // start packager first so it warms up
    console.log(chalk.bold('Starting Packager...'));
    runPackager(true);
    buildAndRun();
  });
};
