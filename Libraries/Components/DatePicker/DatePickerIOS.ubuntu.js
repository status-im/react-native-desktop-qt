/**
 * @providesModule DatePickerIOS
 * @flow
 */
'use strict';

var React = require('React');
var PropTypes = require('ReactPropTypes');
var View = require('View');
var StyleSheetPropType = require("StyleSheetPropType");
var LayoutPropTypes = require("LayoutPropTypes");
var requireNativeComponent = require('requireNativeComponent');

var UbuntuDatePicker = requireNativeComponent('UbuntuDatePicker');

var DatePickerIOS = React.createClass({
  propTypes: {
    style: StyleSheetPropType(LayoutPropTypes),
    date: PropTypes.instanceOf(Date).isRequired,
    onDateChange: PropTypes.func.isRequired,
    maximumDate: PropTypes.instanceOf(Date),
    minimumDate: PropTypes.instanceOf(Date),
    mode: PropTypes.oneOf(['date', 'time', 'datetime']),
    minuteInterval: PropTypes.oneOf([1, 2, 3, 4, 5, 6, 10, 12, 15, 20, 30]),
    timeZoneOffsetInMinutes: PropTypes.number,
  },

  getDefaultProps: function(): DefaultProps {
    return {
      mode: 'date',
    };
  },

  _onChange: function(event: Event) {
    var nativeTimeStamp = event.nativeEvent.timestamp;
    this.props.onDateChange && this.props.onDateChange(
      new Date(nativeTimeStamp)
    );
  },

  render(): ReactElement {
    var { onDateChange, ...props } = this.props;
    return (
      <UbuntuDatePicker
        {...props}
        style={{width: 288, height: 160}} // TODO: grab from units.gu(), implicit* not set?
        onChange={this._onChange} />
    );
  }
});

module.exports = DatePickerIOS;
