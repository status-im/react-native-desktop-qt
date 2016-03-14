/*
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
