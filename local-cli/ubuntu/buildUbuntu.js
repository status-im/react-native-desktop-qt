
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
const fs = require('fs');
const child_process = require('child_process');


function buildUbuntu(args, dependencies) {
  return _findModules(args).then((dependencies) => {
    return new Promise((resolve, reject) => {
      _findUbuntuModules(args, dependencies, resolve, reject);
    });
  }).then((ubuntuModules) => {
    return new Promise((resolve, reject) => {
      if (ubuntuModules && ubuntuModules.length > 0) {
        _buildModules(args, ubuntuModules, resolve, reject);
      } else {
        resolve();
      }
    });
  }).then(() => {
    return _buildApplication(args);
  });
}

function _findModules(args) {
  return new Promise((resolve, reject) => {
    fs.readFile(path.join(args.root, 'package.json'), (err, data) => {
      if (err) {
        reject(err);
      } else {
        resolve(JSON.parse(data).dependencies);
      }
    });
  });
}

function _findUbuntuModules(args, dependencies, resolve, reject) {
  var potential = Object.keys(dependencies).map((p) => {
    if (p === 'react' || p === 'react-native')
      return null;

    return new Promise((resolve, reject) => {
      const depPath = path.join(args.root, 'node_modules', p);
      fs.readFile(path.join(depPath, 'package.json'), (err, data) => {
        const ubuntu = data && JSON.parse(data)._ubuntu;
        if (ubuntu !== undefined && ubuntu.hasOwnProperty('build')) {
          resolve({name: p, build: ubuntu.build, path: depPath});
        } else {
          resolve(null);
        }
      });
    });
  });

  Promise.all(potential).then((result) => {
    resolve(result.filter((v) => v !== null));
  }).catch((err) => {
    reject("Error assessing Ubuntu module: " + err);
  });
}

function _buildModules(args, dependencies, resolve, reject) {
  var builds = dependencies.map((p) => {
    return new Promise((resolve, reject) => {
      console.log(chalk.bold('Building Ubuntu module: ') + p.name);

      var buildCommand = 'URN_OUTPUT_DIR=' + path.resolve(args.root, 'ubuntu', 'plugins');
      if (args['arch'].startsWith('arm')) {
        buildCommand = 'click chroot -a armhf -f ubuntu-sdk-15.04 -n click run ' + buildCommand + ' ' + p.build;
      } else {
        buildCommand += ' ' + p.build;
      }

      child_process.exec(buildCommand, {cwd: p.path}, (error, stdout, stderr) => {
                             if (error) {
                               reject(error);
                             } else {
                               resolve(true);
                             }
                            });
    });
  });

  Promise.all(builds).then((result) => {
    resolve();
  }).catch((err) => {
    reject("Package build failed: " + err);
  });
}

function _buildApplication(args) {
  return new Promise((resolve, reject) => {
    console.log(chalk.bold('Building the app...'));

    var buildCommand = './build.sh';
    if (args['arch'].startsWith('arm')) {
      buildCommand = 'click chroot -a armhf -f ubuntu-sdk-15.04 -n click run ' + buildCommand;
    }
    child_process.exec(buildCommand, {cwd: path.join(args.root, 'ubuntu')},
                        (error, stdout, stderr) => {
                          if (error)
                            reject(error);
                          else
                            resolve();
                        });
  });
}

module.exports = buildUbuntu;
