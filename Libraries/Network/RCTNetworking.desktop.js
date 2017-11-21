/**
 * Copyright (C) 2016, Canonical Ltd.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * @providesModule RCTNetworking
 */

'use strict';

var RCTNetworkingNative = require('NativeModules').Networking;

class RCTNetworking {

  static sendRequest(requestId, method, url, headers, data) {
    RCTNetworkingNative.sendRequest(
      requestId,
      method,
      url,
      headers,
      data);
  }

  static abortRequest(requestId) {
    RCTNetworkingNative.abortRequest(requestId);
  }

}

module.exports = RCTNetworking;
