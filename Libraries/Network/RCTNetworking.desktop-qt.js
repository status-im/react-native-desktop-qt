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
const NativeEventEmitter = require('NativeEventEmitter');
const convertRequestBody = require('convertRequestBody');

class RCTNetworking extends NativeEventEmitter {

  constructor() {
    super(RCTNetworkingNative);
  }

  sendRequest(
    method: string,
    trackingName: string,
    url: string,
    headers: Object,
    data: RequestBody,
    responseType: 'text' | 'base64',
    incrementalUpdates: boolean,
    timeout: number,
    callback: (requestId: number) => any,
    withCredentials: boolean
  ) {
    const body = convertRequestBody(data);
    console.log("Starting call sendRequest on RCTNetworkingNative... method:" + method + " url: " + url);
    RCTNetworkingNative.sendRequest(
      method,
      url,
      {data: {...body, trackingName}},
      headers,
      responseType,
      incrementalUpdates,
      timeout,
      withCredentials,
      callback);
  }

  abortRequest(requestId) {
    RCTNetworkingNative.abortRequest(requestId);
  }
}

 RCTNetworking = new RCTNetworking();

module.exports = RCTNetworking;
