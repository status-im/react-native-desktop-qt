
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

var chalk = require('chalk');
var fs = require('fs');
var path = require('path');
var yeoman = require('yeoman-generator');
var mkdirp = require('mkdirp');


function validatePackageName(name) {
  // TODO: check that this matches Desktop package reqs as well
  if (!name.match(/^([a-zA-Z_$][a-zA-Z\d_$]*\.)+([a-zA-Z_$][a-zA-Z\d_$]*)$/)) {
    return false;
  }
  return true;
}

module.exports = yeoman.generators.NamedBase.extend({
  constructor: function() {
    yeoman.generators.NamedBase.apply(this, arguments);

    this.option('package', {
      desc: 'Package name for the application (appname.developername)',
      type: String,
      defaults: this.name.toLowerCase() + '.dev'
    });
  },

  initializing: function() {
    if (!validatePackageName(this.options.package)) {
      throw new Error('Package name ' + this.options.package + ' is invalid');
    }
  },

  writing: function() {
    var templateParams = {
      package: this.options.package,
      name: this.name,
      lowerCaseName: this.name.toLowerCase()
    };
    this.fs.copyTpl(
      this.templatePath('CMakeLists.txt'),
      this.destinationPath(path.join('desktop', 'CMakeLists.txt')),
      templateParams
    );
    var buildScriptPath = process.platform === "win32" ? "build.bat" : "build.sh";
    this.fs.copyTpl(
      this.templatePath(buildScriptPath),
      this.destinationPath(path.join('desktop', buildScriptPath)),
      templateParams
    );
    var runScriptPath = process.platform === "win32" ? "run-app.bat.in" : "run-app.sh.in";
    this.fs.copyTpl(
      this.templatePath(runScriptPath),
      this.destinationPath(path.join('desktop', runScriptPath)),
      templateParams
    );
    this.fs.copyTpl(
      this.templatePath('ubuntu-server.js'),
      this.destinationPath(path.join('desktop', 'bin/ubuntu-server.js')),
      templateParams
    );

    // Custom application main.cpp source
    this.fs.copyTpl(
      this.templatePath('../../../ReactQt/application/src/main.cpp'),
      this.destinationPath(path.join('desktop', 'main.cpp')),
      templateParams
    );

    mkdirp.sync('desktop/share');
    mkdirp.sync('desktop/plugins');
    mkdirp.sync('desktop/tmp');
  },

  end: function() {
    var projectPath = this.destinationRoot();
    this.log(chalk.white.bold('To run your app on your Desktop natively:'));
    this.log(chalk.white('   cd ' + projectPath));
    this.log(chalk.white('   react-native run-desktop'));
  }
});
