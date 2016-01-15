/**
 * @providesModule MoviesApp
 * @flow
 */
'use strict';

var React = require('react-native');
var {
  AppRegistry,
  Navigator,
  StyleSheet,
} = React;

var SearchScreen = require('./SearchScreen');

var View = require('View');

var RouteMapper = function(route, nav) {
  if (route.name === 'search') {
    return (
      <SearchScreen navigator={nav} />
    );
  } else if (route.name === 'movie') {
    return (
      <View style={{flex: 1}}>
        <MovieScreen
          style={{flex: 1}}
          navigator={navigationOperations}
          movie={route.movie}
        />
      </View>
    );
  }
};

var MoviesApp = React.createClass({
  render: function() {
    return (
      <Navigator
        style={styles.container}
        initialRoute={{ name: 'search'}}
        renderScene={RouteMapper}
      />
    );
  }
});

var styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: 'white',
  },
});

AppRegistry.registerComponent('MoviesApp', () => MoviesApp);

module.exports = MoviesApp;
