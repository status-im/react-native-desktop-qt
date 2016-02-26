/**
 * @flow
 */
'use strict';

var React = require('react-native');
var {
  AppRegistry,
  Settings,
  SnapshotViewIOS,
  StyleSheet,
} = React;

var UIExplorerListBase = require('./UIExplorerListBase');

var COMPONENTS = [
  require('./ActivityIndicatorIOSExample'),
  require('./DatePickerIOSExample'),
  require('./ImageExample'),
  require('./LayoutEventsExample'),
  require('./ListViewExample'),
  require('./ListViewGridLayoutExample'),
  require('./ListViewPagingExample'),
  require('./MapViewExample'),
  require('./ModalExample'),
  require('./Navigator/NavigatorExample'),
  // require('./NavigatorIOSColorsExample'),
  // require('./NavigatorIOSExample'),
  require('./PickerIOSExample'),
  require('./ProgressViewIOSExample'),
  require('./ScrollViewExample'),
  require('./SegmentedControlIOSExample'),
  require('./SliderIOSExample'),
  require('./SwitchIOSExample'),
  require('./TabBarIOSExample'),
  require('./TextExample.ios'),
  require('./TextInputExample.ios'),
  require('./TouchableExample'),
  require('./ViewExample'),
  require('./WebViewExample'),
];

var APIS = [
  require('./AccessibilityIOSExample'),
  require('./ActionSheetIOSExample'),
  require('./AdSupportIOSExample'),
  require('./AlertIOSExample'),
  require('./AnimatedExample'),
  require('./AnimatedGratuitousApp/AnExApp'),
  require('./AppStateIOSExample'),
  require('./AsyncStorageExample'),
  require('./BorderExample'),
  require('./CameraRollExample.ios'),
  require('./GeolocationExample'),
  require('./LayoutExample'),
  require('./NetInfoExample'),
  require('./PanResponderExample'),
  require('./PointerEventsExample'),
  require('./PushNotificationIOSExample'),
  require('./StatusBarIOSExample'),
  require('./TimerExample'),
  require('./VibrationIOSExample'),
  require('./XHRExample.ubuntu'),
  require('./ImageEditingExample'),
];


type Props = {
  navigator: Object,
};

class UIExplorerList extends React.Component {
  props: Props;

  render() {
    return (
      <UIExplorerListBase
          components={COMPONENTS}
          apis={APIS}
          // searchText={Settings.get('searchText')}
          renderAdditionalView={this.renderAdditionalView.bind(this)}
          search={this.search.bind(this)}
          onPressRow={this.onPressRow.bind(this)}
        />
    );
  }

  renderAdditionalView(renderRow: Function, renderTextInput: Function): React.Component {
    return renderTextInput(styles.searchTextInput);
  }

  search(text: mixed) {
    Settings.set({searchText: text});
  }

  _openExample(example: any) {
    if (example.external) {
      this.props.onExternalExampleRequested(example);
      return;
    }

    var Component = UIExplorerListBase.makeRenderable(example);
    this.props.navigator.push({
      title: Component.title,
      component: Component,
    });
  }

  onPressRow(example: any) {
    this._openExample(example);
  }
}

var styles = StyleSheet.create({
  searchTextInput: {
    height: 30,
  },
});

module.exports = UIExplorerList;
