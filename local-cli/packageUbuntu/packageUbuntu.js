
/**
 * Copyright (C) 2016, Canonical Ltd.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

const path = require('path');
const chalk = require('chalk');
const child_process = require('child_process');
const fs = require('fs');
const mkdirp = require('mkdirp');
const parseCommandLine = require('../util/parseCommandLine');
const isPackagerRunning = require('../util/isPackagerRunning');
const Promise = require('promise');
const bundle = require('../bundle/bundle');

module.exports = function(argv, config) {
  mkdirp.sync('ubuntu/click/share/js');
  return new Promise((resolve, reject) => {
          args='--platform ubuntu --entry-file index.ubuntu.js --bundle-output ubuntu/click/share/js/index.js --assets-dest ubuntu/click/share/assets'.split(' ');
          bundle(args, config).then(bundle => {
                child_process.spawnSync('click',
                                        'build ubuntu/click .'.split(' '),
                                        {stdio: 'inherit'});
                resolve();
              });
        })
}

