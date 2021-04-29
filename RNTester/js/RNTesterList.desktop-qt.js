/**
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 *
 * @format
 * @flow
 */

'use strict';

import type {RNTesterExample} from './Shared/RNTesterTypes';

const ComponentExamples: Array<RNTesterExample> = [
  {
    key: 'ActivityIndicatorExample',
    module: require('./ActivityIndicatorExample'),
  },
  // TODO: crashes on start
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
  },
  {
    key: 'FlatListExample',
    module: require('./FlatListExample'),
  },
  {
    key: 'ImageExample',
    module: require('./ImageExample'),
  },
  // Doesn't exist on desktop
  // {
  //   key: 'InputAccessoryViewExample',
  //   module: require('./InputAccessoryViewExample'),
  // },
  // Doesn't exist on desktop
  // {
  //   key: 'KeyboardAvoidingViewExample',
  //   module: require('./KeyboardAvoidingViewExample'),
  // },
  {
    key: 'LayoutEventsExample',
    module: require('./LayoutEventsExample'),
  },
  {
    key: 'MaskedViewExample',
    module: require('./MaskedViewExample'),
  },
  {
    key: 'ModalExample',
    module: require('./ModalExample'),
  },
  {
    key: 'MultiColumnExample',
    module: require('./MultiColumnExample'),
  },
  {
    key: 'NewAppScreenExample',
    module: require('./NewAppScreenExample'),
  },
  {
    key: 'PickerExample',
    module: require('./PickerExample'),
  },
  // {
  //   key: 'PickerIOSExample',
  //   module: require('./PickerIOSExample'),
  // },
  // {
  //   key: 'ProgressViewIOSExample',
  //   module: require('./ProgressViewIOSExample'),
  // },
  {
    key: 'RefreshControlExample',
    module: require('./RefreshControlExample'),
  },
  {
    key: 'ScrollViewSimpleExample',
    module: require('./ScrollViewSimpleExample'),
  },
  {
    key: 'SafeAreaViewExample',
    module: require('./SafeAreaViewExample'),
  },
  {
    key: 'ScrollViewExample',
    module: require('./ScrollViewExample'),
  },
   //TODO: error "native animated module not implemented"
  // {
  //   key: 'SectionListExample',
  //   module: require('./SectionListExample'),
  // },
  // {
  //   key: 'SegmentedControlIOSExample',
  //   module: require('./SegmentedControlIOSExample'),
  // },
  {
    key: 'SliderExample',
    module: require('./SliderExample'),
  },
  {
    key: 'StatusBarExample',
    module: require('./StatusBarExample'),
  },
  {
    key: 'SwitchExample',
    module: require('./SwitchExample'),
  },
  {
    key: 'TextExample',
    module: require('./TextExample.desktop-qt'),
  },
  {
    key: 'TextInputExample',
    module: require('./TextInputExample.desktop-qt'),
  },
  {
    key: 'TouchableExample',
    module: require('./TouchableExample'),
  },
  {
    key: 'TransparentHitTestExample',
    module: require('./TransparentHitTestExample'),
  },
  {
    key: 'ViewExample',
    module: require('./ViewExample'),
  },
];

const APIExamples: Array<RNTesterExample> = [
  // {
  //   key: 'AccessibilityExample',
  //   module: require('./AccessibilityExample'),
  // },
  // {
  //   key: 'AccessibilityIOSExample',
  //   module: require('./AccessibilityIOSExample'),
  // },
  // {
  //   key: 'ActionSheetIOSExample',
  //   module: require('./ActionSheetIOSExample'),
  // },
  {
    key: 'AlertIOSExample',
    module: require('./AlertIOSExample'),
  },
  {
    key: 'AnimatedExample',
    module: require('./AnimatedExample'),
  },
  {
    key: 'AnExApp',
    module: require('./AnimatedGratuitousApp/AnExApp'),
  },
  {
    key: 'AppStateExample',
    module: require('./AppStateExample'),
  },
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
  // },
  {
    key: 'ClipboardExample',
    module: require('./ClipboardExample'),
  },
  {
    key: 'CrashExample',
    module: require('./CrashExample'),
  },
  {
    key: 'Dimensions',
    module: require('./DimensionsExample'),
  },
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
  // {
  //   key: 'OrientationChangeExample',
  //   module: require('./OrientationChangeExample'),
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
  // },
  // {
  //   key: 'SnapshotExample',
  //   module: require('./SnapshotExample'),
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
  //   key: 'TurboModuleExample',
  //   module: require('./TurboModuleExample'),
  // },
  // {
  //   key: 'TVEventHandlerExample',
  //   module: require('./TVEventHandlerExample'),
  // },
  // {
  //   key: 'VibrationExample',
  //   module: require('./VibrationExample'),
  // },
  {
    key: 'WebSocketExample',
    module: require('./WebSocketExample'),
  },
  // {
  //   key: 'XHRExample',
  //   module: require('./XHRExample'),
  // },
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
