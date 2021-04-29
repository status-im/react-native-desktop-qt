/**
 * Copyright (C) 2016, Canonical Ltd.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * @providesModule Navigator
 */
'use strict';

var Map = require('Map');
var React = require('React');
var PropTypes = React.PropTypes;
var LayoutPropTypes = require("LayoutPropTypes");
var StyleSheetPropType = require("StyleSheetPropType");
var createReactClass = require('create-react-class');
var requireNativeComponent = require('requireNativeComponent');
var findNodeHandle = require('findNodeHandle');
var util = require('util');

var {
  ReactNavigatorManager
} = require('NativeModules');

var View = require('View');

var Page = createReactClass({
  propTypes: {
    style: StyleSheetPropType(LayoutPropTypes),
    head: PropTypes.object,
    title: PropTypes.string,
    qmlAnchors: PropTypes.bool
  },

  render: function() {
    return (
        <UbuntuPage
          title={this.props.title}
          qmlAnchors={true}>
          {this.props.children}
        </UbuntuPage>
      );
  }
});

var UbuntuPage = requireNativeComponent('UbuntuPage', Page);


var Navigator = createReactClass({
  propTypes: {
    initialRoute: PropTypes.object,
    renderScene: PropTypes.func.isRequired,
    onBackButtonPress: PropTypes.func,
    numberPages: PropTypes.number
  },

  getInitialState: function() {
    return { pageStack: [] };
  },

  componentWillMount: function() {
    this._navigator = null;
    this._pushPage = null;
    this._popPage = null;
    this._pages = [];
    this._pageRefs = [];
    this._routeMap = new Map();

    if (this.props.initialRoute) {
      this.push(this.props.initialRoute);
    }
  },

  componentDidMount: function() {
    if (this._pushPage == null)
      return;
    ReactNavigatorManager.push(findNodeHandle(this._navigator), findNodeHandle(this._pageRefs[this._pushPage]));
    this._pushPage = null;
  },

  componentDidUpdate: function() {
    console.log("=== componentDidUpdate: pushPage=" + this._pushPage);
    if (this._pushPage != null) {
      console.log("=== componentDidUpdate: will call push(" +
                  findNodeHandle(this._navigator) +", " +
                  findNodeHandle(this._pageRefs[this._pushPage]) + ")");
      ReactNavigatorManager.push(findNodeHandle(this._navigator), findNodeHandle(this._pageRefs[this._pushPage]));
      this._pushPage = null;
    }
    if (this._popPage != null) {
      console.log("=== componentDidUpdate: will call pop()");
      ReactNavigatorManager.pop(findNodeHandle(this._navigator));
      this._popPage = null;
    }
  },

  push: function(route) {
    var newPages = this.state.pageStack.concat([route]);
    this.setState({
        pageStack: newPages
      });
  },

  pop: function() {
    console.log("=== Navigator.pop()");
    var pageStack = this.state.pageStack;
    if (pageStack.length == 0)
      return;

    var route = pageStack[pageStack.length - 1];
    console.log("=== Navigator.pop(); delete route" + util.inspect(route));
    this._routeMap.delete(route);
    var newPages = pageStack.slice(0, -1);
    this._popPage = true;
    this.setState({
        pageStack: newPages
      });
  },

  render: function() {
    var pages = [];
    for (var i in this.state.pageStack) {
      var page = this.state.pageStack[i];
      if (this._routeMap.has(page)) {
        pages.push(this._routeMap.get(page));
      } else {
        var newPage = <Page
                        style={{flex: 1}}
                        ref={(page) => this._pageRefs[i] = page}
                        title={page.title || page.name}>
                        {this.props.renderScene(page, this)}
                      </Page>;
        this._routeMap.set(page, newPage);
        pages.push(newPage);
        this._pushPage = i;
      }
    }
    this._pages = pages;

    return (
      <UbuntuNavigator
        ref={(nav) => this._navigator = nav}
        style={this.props.style}
        onBackButtonPress={() => this.pop()}>
        {this._pages}
      </UbuntuNavigator>
    );
  },
});

var UbuntuNavigator = requireNativeComponent('UbuntuNavigator', Navigator);

module.exports = Navigator;
