
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

const path = require('path');
const chalk = require('chalk');
const child_process = require('child_process');
const fs = require('fs');
const mkdirp = require('mkdirp');
const Promise = require('promise');
const bundle = require('../bundle/bundle');

const parseArguments = require('./parseArguments');
const checkUbuntu = require('./checkUbuntu');
const buildUbuntu = require('./buildUbuntu');


function packageUbuntu(argv, config) {
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
    console.log(chalk.bold('Running RN bundler...'));
    mkdirp.sync(path.join(args.root, 'ubuntu/click/share/js'));
    const bundleArgs='--platform ubuntu --entry-file index.ubuntu.js --bundle-output ubuntu/click/share/js/index.js --assets-dest ubuntu/click/share/assets'.split(' ');
    return bundle(bundleArgs, config);
  }).then(() => {
    return new Promise((resolve, reject) => {
      _copyAssets(args, resolve, reject);
    });
  }).then(() => {
    return new Promise((resolve, reject) => {
      _copyModules(args, resolve, reject);
    });
  }).then(() => {
    return new Promise((resolve, reject) => {
      _buildClick(args, resolve, reject);
    });
  });
}

function _copyAssets(args, resolve, reject) {
  console.log(chalk.bold('Copying app assets...'));
  child_process.exec('cp -uR ' + path.join(args.root, 'ubuntu/share/* ') + path.join(args.root, 'ubuntu/click/share') + ' || true',
                      {}, (error, stdout, stderr) => {
                        if (error)
                          reject(error);
                        else
                          resolve();
                      });
}

function _copyModules(args, resolve, reject) {
  console.log(chalk.bold('Copying Ubuntu modules...'));
  const pluginsDestPath = path.join(args.root, 'ubuntu/click/plugins');
  mkdirp.sync(pluginsDestPath);
  child_process.exec('cp -uR ' + path.join(args.root, 'ubuntu/plugins/* ') + pluginsDestPath + ' || true',
                      {}, (error, stdout, stderr) => {
                        if (error)
                          reject(error);
                        else
                          resolve();
                      });
}

function _buildClick(args, resolve, reject) {
  console.log(chalk.bold('Building click package...'));
  child_process.exec('click build click .',
                     {cwd: path.join(args.root, 'ubuntu')},
                     (error, stdout, stderr) => {
                       if (error)
                        reject(error);
                      else
                        resolve();
                     });
}

module.exports = packageUbuntu;

