'use strict';

var chalk = require('chalk');
var fs = require('fs');
var path = require('path');
var yeoman = require('yeoman-generator');

function validatePackageName(name) {
  // TODO: check that this matches Ubuntu package reqs as well
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
      name: this.name
    };
    this.fs.copyTpl(
      this.templatePath(path.join('src', '**')),
      this.destinationPath('ubuntu'),
      templateParams
    );
    this.fs.copy(
      this.templatePath(path.join('bin', '**')),
      this.destinationPath('ubuntu')
    );
  },

  end: function() {
    var projectPath = this.destinationRoot();
    this.log(chalk.white.bold('To run your app on Ubuntu:'));
    this.log(chalk.white('   Have an Ubuntu emulator running, or a device connected'));
    this.log(chalk.white('   cd ' + projectPath));
    this.log(chalk.white('   react-native run-ubuntu'));
  }
});
