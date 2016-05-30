/*
 * Copyright (C) 2016, Canonical Ltd.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * @providesModule XMLHttpRequest
 * @flow
*/

'use strict';

var FormData = require('FormData');
var RCTNetworking = require('RCTNetworking');
var XMLHttpRequestBase = require('XMLHttpRequestBase');

var requestId = 0;

class XMLHttpRequest extends XMLHttpRequestBase {
  _ubuntuRequestId: ?number;

  constructor() {
    super();
    this._ubuntuRequestId = requestId++;
  }

  sendImpl(method: ?string, url: ?string, headers: Object, data: any): void {
    if (typeof data === 'string') {
      data = {string: data};
    } else if (data instanceof FormData) {
      data = {formData: data.getParts()};
    }

    RCTNetworking.sendRequest(
        this._ubuntuRequestId,
        method,
        url,
        headers,
        data
      );

    this.didCreateRequest(this._ubuntuRequestId);
  }
}

module.exports = XMLHttpRequest;
