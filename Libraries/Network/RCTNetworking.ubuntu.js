/**
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
