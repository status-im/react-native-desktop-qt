/**
 * Copyright (c) 2015-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * @flow
 * @providesModule RNTesterList
 */
'use strict';

export type RNTesterExample = {
  key: string,
  module: Object,
};

const ComponentExamples: Array<RNTesterExample> = [
  {
    key: 'ActivityIndicatorExample',
    module: require('./ActivityIndicatorExample'),
  },
  // TODO: ArtExample crashes on start!
  // {
  //   key: 'ARTExample',
  //   module: require('./ARTExample'),
  // },
  {
    key: 'ButtonExample',
    module: require('./ButtonExample'),
  },
  {
    key: 'DatePickerIOSExample',
    module: require('./DatePickerIOSExample'),
    supportsTVOS: false,
  },
  {
    key: 'FlatListExample',
    module: require('./FlatListExample'),
  },
  {
    key: 'ImageExample',
    module: require('./ImageExample'),
  },
  // {
  //   key: 'InputAccessoryViewExample',
  //   module: require('./InputAccessoryViewExample'),
  //   supportsTVOS: true,
  // },
  // {
  //   key: 'KeyboardAvoidingViewExample',
  //   module: require('./KeyboardAvoidingViewExample'),
  // },
  {
    key: 'LayoutEventsExample',
    module: require('./LayoutEventsExample'),
  },
  {
    key: 'ListViewExample',
    module: require('./ListViewExample'),
  },
  {
    key: 'ListViewGridLayoutExample',
    module: require('./ListViewGridLayoutExample'),
  },
  //TODO: gives error Native animated module is not available
  // {
  //   key: 'ListViewPagingExample',
  //   module: require('./ListViewPagingExample'),
  // },
  //TODO: masked view not implemented
  // {
  //   key: 'MaskedViewExample',
  //   module: require('./MaskedViewExample'),
  // },
  {
    key: 'ModalExample',
    module: require('./ModalExample'),
  },
  {
    key: 'MultiColumnExample',
    module: require('./MultiColumnExample'),
  },
  // {
  //   key: 'NavigatorIOSColorsExample',
  //   module: require('./NavigatorIOSColorsExample'),
  //   supportsTVOS: false,
  // },
  // {
  //   key: 'NavigatorIOSBarStyleExample',
  //   module: require('./NavigatorIOSBarStyleExample'),
  //   supportsTVOS: false,
  // },
  // {
  //   key: 'NavigatorIOSExample',
  //   module: require('./NavigatorIOSExample'),
  //   supportsTVOS: true,
  // },
  {
    key: 'PickerExample',
    module: require('./PickerExample'),
  },
  // {
  //   key: 'PickerIOSExample',
  //   module: require('./PickerIOSExample'),
  //   supportsTVOS: false,
  // },
  // {
  //   key: 'ProgressViewIOSExample',
  //   module: require('./ProgressViewIOSExample'),
  // },
  {
    key: 'RefreshControlExample',
    module: require('./RefreshControlExample'),
  },
  // {
  //   key: 'SafeAreaViewExample',
  //   module: require('./SafeAreaViewExample'),
  //   supportsTVOS: true,
  // },
  {
    key: 'ScrollViewExample',
    module: require('./ScrollViewExample'),
  },
  //TODO: error "native animated module not implemented"
  // {
  //   key: 'SectionListExample',
  //   module: require('./SectionListExample'),
  // },
  //TODO: not implemented
  // {
  //   key: 'SegmentedControlIOSExample',
  //   module: require('./SegmentedControlIOSExample'),
  // },
  {
    key: 'SliderExample',
    module: require('./SliderExample'),
  },
  // {
  //   key: 'StatusBarExample',
  //   module: require('./StatusBarExample'),
  //   supportsTVOS: false,
  // },
  {
    key: 'SwipeableFlatListExample',
    module: require('./SwipeableFlatListExample'),
  },
  {
    key: 'SwipeableListViewExample',
    module: require('./SwipeableListViewExample'),
  },
  {
    key: 'SwitchExample',
    module: require('./SwitchExample'),
  },
  // {
  //   key: 'TabBarIOSExample',
  //   module: require('./TabBarIOSExample'),
  // },
  // {
  //   key: 'TabBarIOSBarStyleExample',
  //   module: require('./TabBarIOSBarStyleExample'),
  // },
  {
    key: 'TextExample',
    module: require('./TextExample'),
  },
  {
    key: 'TextInputExample',
    module: require('./TextInputExample'),
  },
  {
    key: 'TouchableExample',
    module: require('./TouchableExample'),
  },
  // {
  //   key: 'TransparentHitTestExample',
  //   module: require('./TransparentHitTestExample'),
  //   supportsTVOS: false,
  // },
  {
    key: 'ViewExample',
    module: require('./ViewExample'),
  },
  {
    key: 'WebViewExample',
    module: require('./WebViewExample'),
  },
];

const APIExamples: Array<RNTesterExample> = [
  // {
  //   key: 'AccessibilityIOSExample',
  //   module: require('./AccessibilityIOSExample'),
  //   supportsTVOS: false,
  // },
  // {
  //   key: 'ActionSheetIOSExample',
  //   module: require('./ActionSheetIOSExample'),
  //   supportsTVOS: true,
  // },
  {
    key: 'AlertExample',
    module: require('./AlertExample').AlertExample,
  },
  // {
  //   key: 'AlertIOSExample',
  //   module: require('./AlertIOSExample'),
  //   supportsTVOS: true,
  // },
  {
    key: 'AnimatedExample',
    module: require('./AnimatedExample'),
  },
  {
    key: 'AnExApp',
    module: require('./AnimatedGratuitousApp/AnExApp'),
  },
  // {
  //   key: 'AppStateExample',
  //   module: require('./AppStateExample'),
  //   supportsTVOS: true,
  // },
  // {
  //   key: 'AsyncStorageExample',
  //   module: require('./AsyncStorageExample'),
  // },
  {
    key: 'BorderExample',
    module: require('./BorderExample'),
  },
  {
    key: 'BoxShadowExample',
    module: require('./BoxShadowExample'),
  },
  // {
  //   key: 'CameraRollExample',
  //   module: require('./CameraRollExample'),
  //   supportsTVOS: false,
  // },
  {
    key: 'ClipboardExample',
    module: require('./ClipboardExample'),
  },
  {
    key: 'Dimensions',
    module: require('./DimensionsExample'),
  },
  // {
  //   key: 'GeolocationExample',
  //   module: require('./GeolocationExample'),
  //   supportsTVOS: false,
  // },
  // {
  //   key: 'ImageEditingExample',
  //   module: require('./ImageEditingExample'),
  // },
  {
    key: 'LayoutAnimationExample',
    module: require('./LayoutAnimationExample'),
  },
  {
    key: 'LayoutExample',
    module: require('./LayoutExample'),
  },
  {
    key: 'LinkingExample',
    module: require('./LinkingExample'),
  },
  {
    key: 'NativeAnimationsExample',
    module: require('./NativeAnimationsExample'),
  },
  {
    key: 'NetInfoExample',
    module: require('./NetInfoExample'),
  },
  // {
  //   key: 'OrientationChangeExample',
  //   module: require('./OrientationChangeExample'),
  //   supportsTVOS: false,
  // },
  {
    key: 'PanResponderExample',
    module: require('./PanResponderExample'),
  },
  {
    key: 'PointerEventsExample',
    module: require('./PointerEventsExample'),
  },
  // {
  //   key: 'PushNotificationIOSExample',
  //   module: require('./PushNotificationIOSExample'),
  //   supportsTVOS: false,
  // },
  // {
  //   key: 'RCTRootViewIOSExample',
  //   module: require('./RCTRootViewIOSExample'),
  // },
  {
    key: 'RTLExample',
    module: require('./RTLExample'),
  },
  // {
  //   key: 'ShareExample',
  //   module: require('./ShareExample'),
  //   supportsTVOS: true,
  // },
  // {
  //   key: 'SnapshotExample',
  //   module: require('./SnapshotExample'),
  //   supportsTVOS: true,
  // },
  {
    key: 'TimerExample',
    module: require('./TimerExample'),
  },
  {
    key: 'TransformExample',
    module: require('./TransformExample'),
  },
  // {
  //   key: 'TVEventHandlerExample',
  //   module: require('./TVEventHandlerExample'),
  //   supportsTVOS: true,
  // },
  // {
  //   key: 'VibrationExample',
  //   module: require('./VibrationExample'),
  //   supportsTVOS: false,
  // },
  {
    key: 'WebSocketExample',
    module: require('./WebSocketExample'),
  },
  {
    key: 'XHRExample',
    module: require('./XHRExample'),
  },
];

const Modules = {};

APIExamples.concat(ComponentExamples).forEach(Example => {
  Modules[Example.key] = Example.module;
});

const RNTesterList = {
  APIExamples,
  ComponentExamples,
  Modules,
};

module.exports = RNTesterList;
