
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
var utils = require('../generator-utils');
var configUpdater = require("./rn-cli-config-updater");

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

    this.option('upgrade', {
      desc: 'Specify an upgrade',
      type: Boolean,
      defaults: false
    });
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

  // configuring: function() {
  //   utils.copyAndReplace(
  //     this.templatePath('../../../.flowconfig'),
  //     this.destinationPath('.flowconfig'),
  //     {
  //       'Libraries\/react-native\/react-native-interface.js' : 'node_modules/react-native/Libraries/react-native/react-native-interface.js',
  //       '^flow/$' : 'node_modules/react-native/flow\nflow/'
  //     }
  //   );
  // },

  writing: function() {
    var templateParams = {
      package: this.options.package,
      name: this.name,
      lowerCaseName: this.name.toLowerCase()
    };

    if (!this.options.upgrade) {
      this.fs.copyTpl(
        this.templatePath('App.desktop.js'),
        this.destinationPath('App.desktop.js'),
        templateParams
      );
    }

    var configPath = this.destinationPath('rn-cli.config.js');
    if (this.fs.exists(configPath)) {
      try {
        var configContents = this.fs.read(configPath);
        var updatedConfigContents = configUpdater(configContents);
        this.fs.write(configPath, updatedConfigContents);
      } catch (err) {
        console.warn("Error updating rn-cli.config.js: ", err.message);
      }
    } else {
      this.fs.copyTpl(this.templatePath("rn-cli.config.js"), configPath);
    }

    this.fs.copyTpl(
      this.templatePath('CMakeLists.txt'),
      this.destinationPath('desktop/CMakeLists.txt'),
      templateParams
    );
    this.fs.copyTpl(
      this.templatePath('build.sh'),
      this.destinationPath('desktop/build.sh'),
      templateParams
    );
    this.fs.copyTpl(
      this.templatePath('build.bat'),
      this.destinationPath('desktop/build.bat'),
      templateParams
    );
    this.fs.copyTpl(
      this.templatePath('run-app.sh.in'),
      this.destinationPath('desktop/run-app.sh.in'),
      templateParams
    );
    this.fs.copyTpl(
      this.templatePath('run-app.bat.in'),
      this.destinationPath('desktop/run-app.bat.in'),
      templateParams
    );
    this.fs.copyTpl(
      this.templatePath('ubuntu-server.js'),
      this.destinationPath('desktop/bin/ubuntu-server.js'),
      templateParams
    );

    // Custom application main.cpp source
    this.fs.copyTpl(
      this.templatePath('../../../ReactQt/application/src/main.cpp'),
      this.destinationPath('desktop/main.cpp'),
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
