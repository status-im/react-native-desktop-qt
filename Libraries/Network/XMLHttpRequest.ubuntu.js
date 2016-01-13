/*
    * @providesModule XMLHttpRequest
    * @flow
*/
'use strict';

var FormData = require('FormData');
var RCTNetworking = require('NativeModules').Networking;

var XMLHttpRequestBase = require('XMLHttpRequestBase');

var requestId = 0;

class XMLHttpRequest extends XMLHttpRequestBase {
  _requestId: ?number;

  constructor() {
    super();
    this._requestId = requestId++;
  }

  sendImpl(method: ?string, url: ?string, headers: Object, data: any): void {
    if (typeof data === 'string') {
      data = {string: data};
    } else if (data instanceof FormData) {
      data = {formData: data.getParts()};
    }

    RCTNetworking.sendRequest(
        this._requestId,
        method,
        url,
        headers,
        data,
        this.callback.bind(this)
      );
  }

  abortImpl(): void {
    if (this._requestId) {
      RCTNetworking.cancelRequest(this._requestId);
      this._clearSubscriptions();
      this._requestId = null;
    }
  }
}

module.exports = XMLHttpRequest;
