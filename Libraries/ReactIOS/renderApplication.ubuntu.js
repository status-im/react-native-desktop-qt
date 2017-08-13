
/**
 * @providesModule renderApplication
 */
'use strict';

var React = require('React');
var ReactNative = require('ReactNative');
var View = require('View');
var StyleSheet = require('StyleSheet');

var invariant = require('fbjs/lib/invariant');

function renderApplication<D, P, S>(
  RootComponent: ReactClass<D, P, S>,
  initialProps: P,
  rootTag: any
) {
  invariant(
    rootTag,
    'Expect to have a valid rootTag, instead got ', rootTag
  );

  ReactNative.render(
    <View style={styles.appContainer} rootTag={rootTag}>
      <RootComponent
        {...initialProps}
        rootTag={rootTag}
      />
    </View>,
    rootTag
  );
}

var styles = StyleSheet.create({
  appContainer: {
    position: 'absolute',
    left: 0,
    top: 0,
    right: 0,
    bottom: 0,
  },
});

module.exports = renderApplication;
