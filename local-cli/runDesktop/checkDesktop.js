
/**
 * Copyright (C) 2016, Canonical Ltd.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * Author: Justin McPherson <justin.mcpherson@canonical.com>
 *
 */

'use strict';

const path = require('path');
const fs = require('fs');

function checkDesktop(args) {
  return fs.existsSync(path.join(args.root, 'desktop/CMakeLists.txt'));
}

module.exports = checkDesktop;
