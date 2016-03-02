/**
 * @providesModule RCTNetworking
 */
'use strict';

var RCTNetworkingNative = require('NativeModules').Networking;

class RCTNetworking {

  static sendRequest(requestId, method, url, headers, data, callback) {
    RCTNetworkingNative.sendRequest(
      requestId,
      method,
      url,
      headers,
      data,
      callback);
  }

  static abortRequest(requestId) {
    RCTNetworkingNative.abortRequest(requestId);
  }

}

module.exports = RCTNetworking;
