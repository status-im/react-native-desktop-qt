/**
 * Copyright (c) 2015-present, Canonical Ltd.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */
'use strict';

const path = require('path');

function getAssetDestPathUbuntu(asset, scale) {
  var suffix = 'generic';
  const ubuntuFolder = 'drawable-' + suffix;
  // TODO: reuse this logic from https://fburl.com/151101135
  const fileName = (asset.httpServerLocation.substr(1) + '/' + asset.name)
    .toLowerCase()
    .replace(/\//g, '_')           // Encode folder structure in file name
    .replace(/([^a-z0-9_])/g, '')  // Remove illegal chars
    .replace(/^assets_/, '');      // Remove "assets_" prefix

  return path.join(ubuntuFolder, fileName + '.' + asset.type);
}

module.exports = getAssetDestPathUbuntu;
