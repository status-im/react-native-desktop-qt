
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

const http = require('http');
const path = require('path');
const chalk = require('chalk');
const child_process = require('child_process');
const fs = require('fs');
const mkdirp = require('mkdirp');
const Promise = require('promise');
const bundle = require('../bundle/bundle');

const parseArguments = require('./parseArguments');
const checkDesktop = require('./checkDesktop');
const buildDesktop = require('./buildDesktop');


function packageDesktop(argv, config) {
  const args = parseArguments(argv);
  var packagePath = '';

  return new Promise((resolve, reject) => {
    if (!checkDesktop(args)) {
      console.log(chalk.red('Desktop project not found. Maybe run react-native desktop first?'));
    } else {
      resolve();
    }
  }).then(() => {
    return new Promise((resolve, reject) => {
      if (!args.click && !args.snap || args.click && args.snap) {
        console.log(chalk.red('You must be creating either a click or snap package.'));
        return;
      }
      packagePath = path.join(args.root, `desktop/${args.click ? 'click' : 'snap'}/`);
      resolve();
    });
  }).then(() => {
    return buildDesktop(args);
  }).then(() => {
    console.log(chalk.bold('Running RN bundler...'));
    const sharePath = path.join(packagePath, 'share');
    mkdirp.sync(path.join(sharePath, 'js'));
    const bundleArgs=`--platform desktop --entry-file index.desktop.js --bundle-output ${path.join(sharePath, 'js/index.js')} --assets-dest ${path.join(sharePath, 'assets')}`.split(' ');
    return bundle(bundleArgs, config);
  }).then(() => {
    return new Promise((resolve, reject) => {
      if (args.click)
        _fetchArmNode(args, packagePath, resolve, reject);
      else
        resolve();
    });
  }).then(() => {
    return new Promise((resolve, reject) => {
      _copyAssets(args, packagePath, resolve, reject);
    });
  }).then(() => {
    return new Promise((resolve, reject) => {
      _copyModules(args, packagePath, resolve, reject);
    });
  }).then(() => {
    return new Promise((resolve, reject) => {
      _copyBinaries(args, packagePath, resolve, reject);
    });
  }).then(() => {
    return new Promise((resolve, reject) => {
      if (args.click)
        _buildClick(args, packagePath, resolve, reject);
      else {
        _buildSnap(args, packagePath, resolve, reject);
      }
    });
  });
}

function _copyAssets(args, packagePath, resolve, reject) {
  console.log(chalk.bold('Copying app assets...'));
  child_process.exec('cp -uR ' + path.join(args.root, 'desktop/share/* ') + path.join(packagePath, 'share') + ' || true',
                      {}, (error, stdout, stderr) => {
                        if (error)
                          reject(error);
                        else
                          resolve();
                      });
}

function _copyModules(args, packagePath, resolve, reject) {
  console.log(chalk.bold('Copying Desktop modules...'));
  const pluginsDestPath = path.join(packagePath, 'plugins');
  mkdirp.sync(pluginsDestPath);
  child_process.exec('cp -uR ' + path.join(args.root, 'desktop/plugins/* ') + pluginsDestPath + ' || true',
                      {}, (error, stdout, stderr) => {
                        if (error)
                          reject(error);
                        else
                          resolve();
                      });
}

function _copyBinaries(args, packagePath, resolve, reject) {
  console.log(chalk.bold('Copying app binaries...'));
  const binDestPath = path.join(packagePath, 'bin');
  mkdirp.sync(binDestPath);
  child_process.exec('cp -uR ' + path.join(args.root, 'desktop/bin/ubuntu-server.js ') + binDestPath + ' || true',
                      {}, (error, stdout, stderr) => {
                        if (error)
                          reject(error);
                        else
                          resolve();
                      });
}

// https://nodejs.org/dist/v4.4.7/node-v4.4.7-linux-armv7l.tar.xz
function _fetchArmNode(args, packagePath, resolve, reject) {
  console.log(chalk.bold('Downloading arm node binaries...'));
  const binDestPath = path.join(packagePath, 'bin');
  const tmpPath = path.join(args.root, 'desktop/tmp');

  const wget = 'wget -N https://nodejs.org/dist/v4.4.7/node-v4.4.7-linux-armv7l.tar.xz';
  const tar = 'tar xf node-v4.4.7-linux-armv7l.tar.xz node-v4.4.7-linux-armv7l/bin/node';
  const mv = 'mv node-v4.4.7-linux-armv7l/bin/node ../../' + binDestPath;
  const download_cmd = wget + ' && ' + tar + ' && ' + mv;

  mkdirp.sync(binDestPath);
  child_process.exec(download_cmd,
                     {cwd: tmpPath},
                     (error, stdout, stderr) => {
                       if (error)
                        reject(error);
                      else
                        resolve();
                     });
}

function _buildClick(args, packagePath, resolve, reject) {
  console.log(chalk.bold('Building click package...'));
  child_process.exec('click build click .',
                     {cwd: path.join(args.root, 'desktop')},
                     (error, stdout, stderr) => {
                       if (error)
                        reject(error);
                      else
                        resolve();
                     });
}

function _buildSnap(args, packagePath, resolve, reject) {
  console.log(chalk.bold('Building snap package...'));
  child_process.exec('snapcraft',
                     {cwd: path.join(args.root, 'desktop/snap')},
                     (error, stdout, stderr) => {
                       if (error)
                        reject(error);
                      else
                        resolve();
                     });
}

module.exports = packageDesktop;

