/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

'use strict';

const React = require('react');
const ReactNative = require('react-native');
const StyleSheet = require('StyleSheet');
const RNTesterBlock = require('RNTesterBlock');
const RNTesterPage = require('RNTesterPage');

const {
  Picker,
  Text,
} = ReactNative;

const Item = Picker.Item;

class PickerExample extends React.Component {
  static title = '<Picker>';

  state = {
    selectedValue: "",
    selectedIndex: ""
  };

  render() {
    return (
      <RNTesterPage title="<Picker>">
        <RNTesterBlock title="Basic Picker">
          <Picker
            style={styles.picker}
            selectedValue="red"
            onValueChange={ (val, pos) => {
                const newState = {};
                newState["selectedValue"] = val;
                newState["selectedIndex"] = pos;
                this.setState(newState);
            }}>
            <Item label="red" color="red" value="red" />
            <Item label="green" color="green" value="green" />
            <Item label="blue" color="blue" value="blue" />
            <Item label="grey" color="grey" value="grey" />
          </Picker>
          <Text>
             Value: {this.state.selectedValue}
          </Text>
          <Text>
             Index: {this.state.selectedIndex}
          </Text>
        </RNTesterBlock>
      </RNTesterPage>
    );
  }
}

var styles = StyleSheet.create({
  picker: {
    width: 100,
    height: 200
  },
});

module.exports = PickerExample;
