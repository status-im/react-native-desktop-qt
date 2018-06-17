/**
 * Copyright (c) 2015-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

var inquirer = require('inquirer');

module.exports = (questions) => new Promise((resolve, reject) => {
  if (!questions) {
    return resolve({});
  }

  inquirer.prompt(questions).then(resolve, reject);
});
