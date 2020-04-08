'use strict';
const chalk = require('chalk');
const path = require('path');
const uuid = require('uuid');
const childProcess = require('child_process');
const os = require('os');

const {
  createDir,
  copyAndReplaceAll,
  copyAndReplaceWithChangedCallback
} = require('../generator-common');

const desktopDir = 'desktop';

function copyProjectTemplateAndReplace(
  srcPath,
  destPath,
  newProjectName,
  options = {}
) {

  if (!srcPath) {
    throw new Error('Need a path to copy from');
  }

  if (!destPath) {
    throw new Error('Need a path to copy to');
  }

  if (!newProjectName) {
    throw new Error('Need a project name');
  }

  createDir(path.join(destPath, desktopDir));


  const templateVars = {
    '<%=name%>': newProjectName
  };

  [
    { from: path.join(srcPath, 'CMakeLists.txt'), to: path.join(desktopDir, 'CMakeLists.txt') },
    { from: path.join(srcPath, 'build.bat'), to: path.join(desktopDir, 'build.bat') },
    { from: path.join(srcPath, 'build.sh'), to: path.join(desktopDir, 'build.sh') },
    { from: path.join(srcPath, 'main.cpp'), to: path.join(desktopDir, 'main.cpp') },
    { from: path.join(srcPath, 'run-app.bat.in'), to: path.join(desktopDir, 'run-app.bat.in') },
    { from: path.join(srcPath, 'run-app.sh.in'), to: path.join(desktopDir, 'run-app.sh.in') },
  ].forEach((mapping) => copyAndReplaceWithChangedCallback(mapping.from, destPath, mapping.to, templateVars));


  console.log(chalk.white.bold('To run your app on desktop:'));
  console.log(chalk.white('   react-native run-desktop'));
}

module.exports = {
  copyProjectTemplateAndReplace
};
