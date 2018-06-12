
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

var fs = require('fs');
var path = require('path');
var yeoman = require('yeoman-environment');

/**
 * Simple utility for running the desktop yeoman generator.
 *
 * @param  {String} projectDir root project directory (i.e. contains index.js)
 * @param  {String} name       name of the root JS module for this app
 */
module.exports = function(projectDir, name) {
  var oldCwd = process.cwd();
  process.chdir(projectDir);

  var env = yeoman.createEnv();
  var generatorPath = path.join(__dirname, 'generator');
  env.register(generatorPath, 'react:desktop');
  var args = ['react:desktop', name].concat(process.argv.slice(4));
  env.run(args, {}, function() {
    process.chdir(oldCwd);
  });
};
