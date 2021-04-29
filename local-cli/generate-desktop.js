'use strict';

const fs = require('fs');
const path = require('path');
const copyProjectTemplateAndReplace = require('./generator-desktop').copyProjectTemplateAndReplace;

/**
 * Simple utility for running the Windows generator.
 *
 * @param  {String} projectDir root project directory (i.e. contains index.js)
 * @param  {String} name       name of the root JS module for this app
 */
function generateDesktop (projectDir, name) {

  if (!fs.existsSync(projectDir)) {
    fs.mkdirSync(projectDir);
  }
  copyProjectTemplateAndReplace(
    path.join(__dirname, 'generator-desktop', 'templates'),
    projectDir,
    name
  );
}

module.exports = generateDesktop;
