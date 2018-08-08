
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


function build(args, dependencies) {
  var desktopExternalModules = _findDesktopExternalModules(args);
  console.log("Found desktop external modules: " + desktopExternalModules);
  var desktopJSBundlePath = _findDesktopJSBundlePath(args);
  console.log("Found desktop JS bundle path: " + desktopJSBundlePath);
  var desktopFonts = _findDesktopFonts(args);
  console.log("Found desktop fonts: " + desktopFonts);

  return _findModules(args).then((dependencies) => {
    return new Promise((resolve, reject) => {
      _findDesktopModules(args, dependencies, resolve, reject);
    });
  }).then((desktopModules) => {
    return new Promise((resolve, reject) => {
      if (desktopModules && desktopModules.length > 0) {
        _buildModules(args, desktopModules, resolve, reject);
      } else {
        resolve();
      }
    });
  }).then(() => {
    return _buildApplication(args, desktopExternalModules, desktopJSBundlePath, desktopFonts);
  });
}

function _findDesktopExternalModules(args) {
  var data = fs.readFileSync(path.join(args.root, 'package.json'));
  return JSON.parse(data).desktopExternalModules;
}

function _findDesktopJSBundlePath(args) {
  var data = fs.readFileSync(path.join(args.root, 'package.json'));
  return JSON.parse(data).desktopJSBundlePath;
}

function _findDesktopFonts(args) {
  var data = fs.readFileSync(path.join(args.root, 'package.json'));
  return JSON.parse(data).desktopFonts;
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

function _findDesktopModules(args, dependencies, resolve, reject) {
  var potential = Object.keys(dependencies).map((p) => {
    if (p === 'react' || p === 'react-native')
      return null;

    return new Promise((resolve, reject) => {
      const depPath = path.join(args.root, 'node_modules', p);
      fs.readFile(path.join(depPath, 'package.json'), (err, data) => {
        const desktop = data && JSON.parse(data)._desktop;
        if (desktop !== undefined && desktop.hasOwnProperty('build')) {
          resolve({name: p, build: desktop.build, path: depPath});
        } else {
          resolve(null);
        }
      });
    });
  });

  Promise.all(potential).then((result) => {
    resolve(result.filter((v) => v !== null));
  }).catch((err) => {
    reject("Error assessing Desktop module: " + err);
  });
}

function _buildModules(args, dependencies, resolve, reject) {
  var builds = dependencies.map((p) => {
    return new Promise((resolve, reject) => {
      console.log(chalk.bold('Building Desktop module: ') + p.name);

      var buildCommand = 'URN_OUTPUT_DIR=' + path.resolve(args.root, 'desktop', 'plugins');
      buildCommand += ' ' + p.build;

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

function _buildApplication(args, desktopExternalModules, desktopJSBundlePath, desktopFonts) {
  return new Promise((resolve, reject) => {
    console.log(chalk.bold('Building the app...'));

    var buildCommand = process.platform === "win32" ? "build.bat" : "./build.sh";
    if (typeof desktopExternalModules !== 'undefined' && desktopExternalModules !== null) {
      buildCommand += ' -e "' + desktopExternalModules.toString().replace(/,/g, ';') + '"';
    }
    if (typeof desktopJSBundlePath !== 'undefined' && desktopJSBundlePath !== null) {
      buildCommand += ' -j "' + desktopJSBundlePath.toString() + '"';
    }
    if (typeof desktopFonts !== 'undefined' && desktopFonts !== null) {
      buildCommand += ' -f "' + desktopFonts.toString().replace(/,/g, ';') + '"';
    }
    if (process.platform === "win32") {
      buildCommand += ' -g "' + "MinGW Makefiles" + '"';
    }
    child_process.exec(buildCommand, {cwd: path.join(args.root, 'desktop')},
                        (error, stdout, stderr) => {
                          if (error)
                            reject(error);
                          else {
                            console.log(stdout);
                            resolve();
                          }
                        });
  });
}

module.exports = build;
