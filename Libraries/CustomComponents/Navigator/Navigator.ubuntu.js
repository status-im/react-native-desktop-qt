/**
 * @providesModule Navigator
 */
'use strict';

var Map = require('Map');
var React = require('React');
var PropTypes = React.PropTypes;

var requireNativeComponent = require('requireNativeComponent');
var findNodeHandler = require('findNodeHandle');

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
    UbuntuPageStackManager.push(findNodeHandle(this), page);
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
    this._pages = [];
    this._routeMap = new Map();

    if (this.props.initialRoute) {
      this.setState({
        pageStack: [this.props.initialRoute]
      });
    }
  },

  push: function(route) {
    var newPages = [route].concat(this.state.routeStack);
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
    for (var page in this.state.pageStack) {
      var route = this.state.pageStack[page];
      if (this._routeMap.has(route)) {
        pages.push(this._routeMap.get(route));
      } else {
        var newPage = <Page>
                        {this.props.renderScene(route, this)}
                      </Page>;
        this._routeMap.set(route, newPage);
        pages.push(newPage);
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
