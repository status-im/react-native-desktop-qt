/*
    * @providesModule XMLHttpRequest
    * @flow
*/
'use strict';

var FormData = require('FormData');
var RCTNetworking = require('NativeModules').Networking;

var XMLHttpRequestBase = require('XMLHttpRequestBase');

class XMLHttpRequest extends XMLHttpRequestBase {

  _requestId: ?number;
  _subscriptions: [any];
  upload: {
    onprogress?: (event: Object) => void;
  };

  constructor() {
    super();
    this._requestId = null;
    this._subscriptions = [];
    this.upload = {};
  }

  sendImpl(method: ?string, url: ?string, headers: Object, data: any): void {
    if (typeof data === 'string') {
      data = {string: data};
    } else if (data instanceof FormData) {
      data = {formData: data.getParts()};
    }
    RCTNetworking.sendRequest(
      {
        method,
        url,
        data,
        headers,
        incrementalUpdates: this.onreadystatechange ? true : false,
      });
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
