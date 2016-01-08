/**
 * @providesModule Navigator
 */
'use strict';

var Map = require('Map');
var React = require('React');
var PropTypes = React.PropTypes;

var requireNativeComponent = require('requireNativeComponent');
var findNodeHandler = require('findNodeHandle');

var {
  UbuntuMainViewManager,
  UbuntuPageStackManager,
  UbuntuPageManager
} = require('NativeModules');

var MainView = React.createClass({
  propTypes: {
    automaticOrientation: PropTypes.bool,
    contentsItem: PropTypes.array
  },

  render: function() {
    return (<UbuntuMainView />);
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
    return (<UbuntuPageStack />);
  }
});

var UbuntuPageStack = requireNativeComponent('UbuntuPageStack', PageStack);


var Page = React.createClass({
  propTypes: {
    head: PropTypes.object
  },

  render: function() {
    return (<UbuntuPage />);
  }
});

var UbuntuPage = requireNativeComponent('UbuntuPage', Page);


var Navigator = React.createClass({
  propTypes: {
    initialRoute: PropTypes.object,
    renderScene: PropTypes.func.isRequired,
  },

  getInitialState: function() {
    return {};
  },

  componentWillMount: function() {
    this._routeStack = [];
    this._pageStack = <PageStack ref='pageStack'/>;
    if (this.props.initialRoute) {
//        util.inspect(this._pageStack, { colors: true });
        console.log("" + PageStack);
//      this._pageStack.push(this.props.initialRoute);
      this.refs.pageStack.push(this.initialRoute);
    }
  },

  push: function(route) {
    this._routeStack.unshift(route);
    var newPage = <Page>
                    {this.props.renderScene(route, this)}
                  </Page>;
    this._pageStack.push(newPage);
    this.setState({
          activePage: newPage
        });
  },

  pop: function() {
    var route = this._pageStack.shift();
    this.setState({
        activePage: this._routeStack[0]
      });
  },

  render: function() {
    return (
      <MainView>
        {this._pageStack}
      </MainView>
    );
  },
});

module.exports = Navigator;

