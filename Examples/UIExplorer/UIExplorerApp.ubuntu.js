/**
 * The examples provided by Facebook are for non-commercial testing and
 * evaluation purposes only.
 *
 * Facebook reserves all rights not expressly granted.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL
 * FACEBOOK BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * @providesModule UIExplorerApp
 * @flow
 */
'use strict';

var React = require('react-native');
var {
  AppRegistry,
  StyleSheet,
  View,
  Navigator,
} = React;

var UIExplorerList = require('./UIExplorerList.ubuntu');

var RouteMapper = function(route, nav) {
  if (route.name === 'UIExplorerList') {
    return (
      <UIExplorerList navigator={nav}/>
    );
  } else if (route.hasOwnProperty('component')) {
    return <route.component />
  }
};

var UIExplorerApp = React.createClass({
  render: function() {
    return (
      <Navigator
        style={styles.container}
        initialRoute={{ name: 'UIExplorerList'}}
        renderScene={RouteMapper}
      />
    );
  },
});

var styles = StyleSheet.create({
  container: {
    flex: 1,
  },
});

AppRegistry.registerComponent('UIExplorerApp', () => UIExplorerApp);

module.exports = UIExplorerApp;
