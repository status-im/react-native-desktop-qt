/**
 * Copyright (C) 2016, Canonical Ltd.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * @providesModule Platform
 * @flow
 */

'use strict';

var Platform = {
  OS: 'ubuntu',
  select: (obj: Object) => 'ubuntu' in obj ? obj.ubuntu : obj.default,
};

module.exports = Platform;
