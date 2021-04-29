/**
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 *
 *
 * This is a controlled component version of RCTPickerDesktop
 *
 * @format
 * @flow
 */

'use strict';

const React = require('React');
const ReactNative = require('ReactNative');
const StyleSheet = require('StyleSheet');
const View = require('View');
const processColor = require('processColor');
const requireNativeComponent = require('requireNativeComponent');

import type {SyntheticEvent} from 'CoreEventTypes';
import type {ColorValue} from 'StyleSheetTypes';
import type {ViewProps} from 'ViewPropTypes';
import type {TextStyleProp} from 'StyleSheet';

type PickerDesktopChangeEvent = SyntheticEvent<
  $ReadOnly<{|
    newValue: any,
    newIndex: number,
  |}>,
>;

type RCTPickerDesktopItemType = $ReadOnly<{|
  label: ?Label,
  value: ?any,
  textColor: ?number,
|}>;

type RCTPickerDesktopType = Class<
  ReactNative.NativeComponent<
    $ReadOnly<{|
      items: $ReadOnlyArray<RCTPickerDesktopItemType>,
      onChange: (event: PickerDesktopChangeEvent) => void,
      onResponderTerminationRequest: () => boolean,
      onStartShouldSetResponder: () => boolean,
      selectedIndex: number,
      style?: ?TextStyleProp,
      testID?: ?string,
    |}>,
  >,
>;

const RCTPickerDesktop: RCTPickerDesktopType = (requireNativeComponent(
  'RCTPickerView',
): any);

type Label = Stringish | number;

type Props = $ReadOnly<{|
  ...ViewProps,
  children: React.ChildrenArray<React.Element<typeof PickerDesktopItem>>,
  itemStyle?: ?TextStyleProp,
  onChange?: ?(event: PickerDesktopChangeEvent) => mixed,
  onValueChange?: ?(newValue: any, newIndex: number) => mixed,
  selectedValue: any,
|}>;

type State = {|
  selectedIndex: number,
  items: $ReadOnlyArray<RCTPickerDesktopItemType>,
|};

type ItemProps = $ReadOnly<{|
  label: ?Label,
  value?: ?any,
  color?: ?ColorValue,
|}>;

const PickerDesktopItem = (props: ItemProps) => {
  return null;
};

class PickerDesktop extends React.Component<Props, State> {
  _picker: ?React.ElementRef<RCTPickerDesktopType> = null;

  state = {
    selectedIndex: 0,
    items: [],
  };

  static Item = PickerDesktopItem;

  static getDerivedStateFromProps(props: Props): State {
    let selectedIndex = 0;
    const items = [];
    React.Children.toArray(props.children).forEach(function(child, index) {
      if (child.props.value === props.selectedValue) {
        selectedIndex = index;
      }
      items.push({
        value: child.props.value,
        label: child.props.label,
        textColor: processColor(child.props.color),
      });
    });
    return {selectedIndex, items};
  }

  render() {
    return (
      <View style={this.props.style}>
        <RCTPickerDesktop
          ref={picker => {
            this._picker = picker;
          }}
          testID={this.props.testID}
          style={[styles.pickerDesktop, this.props.itemStyle]}
          items={this.state.items}
          selectedIndex={this.state.selectedIndex}
          onChange={this._onChange}
          onStartShouldSetResponder={() => true}
          onResponderTerminationRequest={() => false}
        />
      </View>
    );
  }

  _onChange = event => {
    if (this.props.onChange) {
      this.props.onChange(event);
    }
    if (this.props.onValueChange) {
      this.props.onValueChange(
        event.nativeEvent.newValue,
        event.nativeEvent.newIndex,
      );
    }

    // The picker is a controlled component. This means we expect the
    // on*Change handlers to be in charge of updating our
    // `selectedValue` prop. That way they can also
    // disallow/undo/mutate the selection of certain values. In other
    // words, the embedder of this component should be the source of
    // truth, not the native component.
    if (
      this._picker &&
      this.state.selectedIndex !== event.nativeEvent.newIndex
    ) {
      this._picker.setNativeProps({
        selectedIndex: this.state.selectedIndex,
      });
    }
  };
}

const styles = StyleSheet.create({
  pickerDesktop: {
    // The picker will conform to whatever width is given, but we do
    // have to set the component's height explicitly on the
    // surrounding view to ensure it gets rendered.
    height: 216,
  },
});

module.exports = PickerDesktop;



// /**
//  * Copyright (c) 2015-present, Facebook, Inc.
//  * All rights reserved.
//  *
//  * This source code is licensed under the BSD-style license found in the
//  * LICENSE file in the root directory of this source tree. An additional grant
//  * of patent rights can be found in the PATENTS file in the same directory.
//  *
//  * @providesModule PickerUbuntu
//  * @flow
//  */
//
// 'use strict';
//
// var ColorPropType = require('ColorPropType');
// var React = require('React');
// var ReactPropTypes = require('prop-types');
// var StyleSheet = require('StyleSheet');
// var StyleSheetPropType = require('StyleSheetPropType');
// const ViewPropTypes = require('ViewPropTypes');
// var ViewStylePropTypes = require('ViewStylePropTypes');
//
// var requireNativeComponent = require('requireNativeComponent');
//
// var REF_PICKER = 'picker';
// var MODE_DROPDOWN = 'dropdown';
//
// var pickerStyleType = StyleSheetPropType({
//   ...ViewStylePropTypes,
//   color: ColorPropType,
// });
//
// type Event = Object;
//
// /**
//  * Not exposed as a public API - use <Picker> instead.
//  */
// class PickerUbuntu extends React.Component {
//   props: {
//     style?: $FlowFixMe,
//     selectedValue?: any,
//     enabled?: boolean,
//     mode?: 'dialog' | 'dropdown',
//     onValueChange?: Function,
//     prompt?: string,
//     testID?: string,
//   };
//
//   state: *;
//
//   static propTypes = {
//     ...ViewPropTypes,
//     style: pickerStyleType,
//     selectedValue: ReactPropTypes.any,
//     enabled: ReactPropTypes.bool,
//     mode: ReactPropTypes.oneOf(['dialog', 'dropdown']),
//     onValueChange: ReactPropTypes.func,
//     prompt: ReactPropTypes.string,
//     testID: ReactPropTypes.string,
//   };
//
//   constructor(props, context) {
//     super(props, context);
//     var state = this._stateFromProps(props);
//
//     this.state = {
//       ...state,
//       initialSelectedIndex: state.selectedIndex,
//     };
//   }
//
//   componentWillReceiveProps(nextProps) {
//     this.setState(this._stateFromProps(nextProps));
//   }
//
//   // Translate prop and children into stuff that the native picker understands.
//   _stateFromProps = (props) => {
//     var selectedIndex = 0;
//     const items = React.Children.map(props.children, (child, index) => {
//       if (child.props.value === props.selectedValue) {
//         selectedIndex = index;
//       }
//       const childProps = {
//         value: child.props.value,
//         label: child.props.label,
//         color: child.props.color
//       };
//       return childProps;
//     });
//     return {selectedIndex, items};
//   };
//
//   render() {
//     var Picker = UbuntuPicker //this.props.mode === MODE_DROPDOWN ? DropdownPicker : DialogPicker;
//     var nativeProps = {
//       enabled: this.props.enabled,
//       items: this.state.items,
//       mode: this.props.mode,
//       onValueChange: this._onChange,
//       prompt: this.props.prompt,
//       selected: this.state.initialSelectedIndex,
//       testID: this.props.testID,
//       style: [styles.pickerUbuntu, this.props.style],
//       accessibilityLabel: this.props.accessibilityLabel,
//     };
//
//     return <Picker ref={REF_PICKER} {...nativeProps} />;
//   }
//
//   _onChange = (event: Event) => {
//     if (this.props.onValueChange) {
//       var position = event.nativeEvent.position;
//       if (position >= 0) {
//         var children = React.Children.toArray(this.props.children);
//         var value = children[position].props.value;
//         this.props.onValueChange(value, position);
//       } else {
//         this.props.onValueChange(null, position);
//       }
//     }
//     this._lastNativePosition = event.nativeEvent.position;
//     this.forceUpdate();
//   };
//
//   componentDidMount() {
//     this._lastNativePosition = this.state.initialSelectedIndex;
//   }
//
//   componentDidUpdate() {
//     // The picker is a controlled component. This means we expect the
//     // on*Change handlers to be in charge of updating our
//     // `selectedValue` prop. That way they can also
//     // disallow/undo/mutate the selection of certain values. In other
//     // words, the embedder of this component should be the source of
//     // truth, not the native component.
//     if (this.refs[REF_PICKER] && this.state.selectedIndex !== this._lastNativePosition) {
//       this.refs[REF_PICKER].setNativeProps({selected: this.state.selectedIndex});
//       this._lastNativePosition = this.state.selectedIndex;
//     }
//   }
// }
//
// var styles = StyleSheet.create({
//   pickerUbuntu: {
//     // The picker will conform to whatever width is given, but we do
//     // have to set the component's height explicitly on the
//     // surrounding view to ensure it gets rendered.
//     // TODO would be better to export a native constant for this,
//     // like in iOS the RCTDatePickerManager.m
//     height: 50,
//   },
// });
//
// var cfg = {
//   nativeOnly: {
//     items: true,
//     selected: true,
//   }
// };
//
// //var DropdownPicker = requireNativeComponent('AndroidDropdownPicker', PickerAndroid, cfg);
// //var DialogPicker = requireNativeComponent('AndroidDialogPicker', PickerAndroid, cfg);
// //var UbuntuPicker = requireNativeComponent('RCTPickerView', Picker, cfg);
// var UbuntuPicker = requireNativeComponent('RCTPickerView');
//
// module.exports = PickerUbuntu;
