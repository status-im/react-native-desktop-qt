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

const React = require('react');
const ReactNative = require('react-native');
const UIExplorerActions = require('./UIExplorerActions');
const UIExplorerList = require('./UIExplorerList.desktop');
const UIExplorerExampleList = require('./UIExplorerExampleList');
const UIExplorerNavigationReducer = require('./UIExplorerNavigationReducer');
const UIExplorerStateTitleMap = require('./UIExplorerStateTitleMap');

const {
  AppRegistry,
  NavigationExperimental,
  StyleSheet,
  View,
} = ReactNative;

const {
  CardStack: NavigationCardStack,
  Header: NavigationHeader,
  Reducer: NavigationReducer,
  RootContainer: NavigationRootContainer,
} = NavigationExperimental;


import type { UIExplorerNavigationState } from './UIExplorerNavigationReducer';

import type { UIExplorerExample } from './UIExplorerList.desktop';


class UIExplorerApp extends React.Component {
  _navigationRootRef: ?NavigationRootContainer;
  _renderNavigation: Function;
  _renderOverlay: Function;
  _renderScene: Function;

  componentWillMount() {
    this._renderNavigation = this._renderNavigation.bind(this);
    this._renderOverlay = this._renderOverlay.bind(this);
    this._renderScene = this._renderScene.bind(this);
  }

  render() {
    return (
      <NavigationRootContainer
        persistenceKey="UIExplorerState"
        reducer={UIExplorerNavigationReducer}
        ref={navRootRef => { this._navigationRootRef = navRootRef; }}
        renderNavigation={this._renderNavigation}
      />
    );
  }

  _renderNavigation(navigationState: UIExplorerNavigationState, onNavigate: Function) {
    if (!navigationState) {
      return null;
    }
    if (navigationState.externalExample) {
      var Component = UIExplorerList.Modules[navigationState.externalExample];
      return (
        <Component
          onExampleExit={() => {
            onNavigate(NavigationRootContainer.getBackAction());
          }}
        />
      );
    }
    const {stack} = navigationState;
    return (
      <NavigationCardStack
        navigationState={stack}
        style={styles.container}
        renderOverlay={this._renderOverlay}
        renderScene={this._renderScene}
      />
    );
  }

  _renderOverlay(props: NavigationSceneRendererProps): ReactElement {
    return (
      <NavigationHeader
        {...props}
        key={'header_' + props.scene.navigationState.key}
        getTitle={UIExplorerStateTitleMap}
      />
    );
  }

  _renderScene(props: NavigationSceneRendererProps): ?ReactElement {
    const state = props.scene.navigationState;
    if (state.key === 'AppList') {
      return (
        <UIExplorerExampleList
          list={UIExplorerList}
          style={styles.exampleContainer}
          {...state}
        />
      );
    }

    const Example = UIExplorerList.Modules[state.key];
    if (Example) {
      const Component = UIExplorerExampleList.makeRenderable(Example);
      return (
        <View style={styles.exampleContainer}>
          <Component />
        </View>
      );
    }
    return null;
  }
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
  },
  exampleContainer: {
    flex: 1,
    paddingTop: 60,
  },
});

AppRegistry.registerComponent('UIExplorerApp', () => UIExplorerApp);

module.exports = UIExplorerApp;
