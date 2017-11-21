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

function getAssetDestPathDesktop(asset, scale) {
  return path.join('share', asset.httpServerLocation.substr(1), asset.name + '.' + asset.type);
}

module.exports = getAssetDestPathDesktop;
