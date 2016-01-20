/**
 * @providesModule Navigator
 */
'use strict';

var Map = require('Map');
var React = require('React');
var PropTypes = React.PropTypes;

var requireNativeComponent = require('requireNativeComponent');
var findNodeHandle = require('findNodeHandle');

var {
  UbuntuPageStackManager
} = require('NativeModules');

var View = require('View');


var MainView = React.createClass({
  propTypes: {
    automaticOrientation: PropTypes.bool,
    contentsItem: PropTypes.array
  },

  render: function() {
      return (
            <UbuntuMainView>
              {this.props.children}
            </UbuntuMainView>
      );
  }
});

var UbuntuMainView = requireNativeComponent('UbuntuMainView', MainView);


var PageStack = React.createClass({
  propTypes: {
    __showHeader: PropTypes.bool,
    currentPage: PropTypes.object,
    data: PropTypes.array,
    depth: PropTypes.number
  },

  push: function(page) {
    UbuntuPageStackManager.push(findNodeHandle(this), {reactTag: page}, {});
  },

  pop: function() {
    UbuntuPageStackManager.pop(findNodeHandle(this));
  },

  clear: function() {
    UbuntuPageStackManager.clear(findNodeHandle(this));
  },

  render: function () {
    return (
        <UbuntuPageStack>
          {this.props.children}
        </UbuntuPageStack>
      );
  }
});

var UbuntuPageStack = requireNativeComponent('UbuntuPageStack', PageStack);

var Page = React.createClass({
  propTypes: {
    head: PropTypes.object
  },

  render: function() {
    return (
        <UbuntuPage>
          {this.props.children}
        </UbuntuPage>
      );
  }
});

var UbuntuPage = requireNativeComponent('UbuntuPage', Page);


var Navigator = React.createClass({
  propTypes: {
    initialRoute: PropTypes.object,
    renderScene: PropTypes.func.isRequired,
  },

  getInitialState: function() {
    return { pageStack: [] };
  },

  componentWillMount: function() {
    this._pageStack = null;
    this._pushPage = null;
    this._pages = [];
    this._pageRefs = [];
    this._routeMap = new Map();

    if (this.props.initialRoute) {
      this.push(this.props.initialRoute);
    }
  },

  componentDidMount: function() {
    console.log("=== componentDidMount");
    if (this._pushPage == null)
      return;
    this._pageStack.push(findNodeHandle(this._pageRefs[this._pushPage]));
    this._pushPage = null;
  },

  componentDidUpdate: function() {
    console.log("=== componentDidUpdate");
    if (this._pushPage == null)
      return;
    this._pageStack.push(findNodeHandle(this._pageRefs[this._pushPage]));
    this._pushPage = null;
  },

  push: function(route) {
    var newPages = [route].concat(this.state.pageStack);
    this.setState({
        pageStack: newPages
      });
  },

  pop: function() {
    var route = this.state.pageStack[0];
    this._routeMap.delete(route);
    var newPages = this.state.pageStack.slice(1);
    this.setState({
        pageStack: newPages
      });
  },

  render: function() {
    var pages = [];
      console.log("=== pageStack="+ JSON.stringify(this.state.pageStack));
    for (var i in this.state.pageStack) {
      var page = this.state.pageStack[i];
      console.log("=== page="+ JSON.stringify(page));
      if (this._routeMap.has(page)) {
        pages.push(this._routeMap.get(page));
      } else {
        var newPage = <Page ref={(page) => this._pageRefs[i] = page}>
                        {this.props.renderScene(page, this)}
                      </Page>;
        this._routeMap.set(page, newPage);
        pages.push(newPage);
        this._pushPage = i;
      }
    }
    this._pages = pages;

    return (
      <View style={this.props.style}>
        <MainView style={this.props.style}>
          <PageStack ref={(stack) => this._pageStack = stack}>
            {this._pages}
          </PageStack>
        </MainView>
      </View>
    );
  },
});

module.exports = Navigator;
