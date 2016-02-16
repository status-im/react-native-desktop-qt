 /*
 * @providesModule ScrollView
 @ @flow
 */

'use strict';

var React = require('React');
var requireNativeComponent = require('requireNativeComponent');

var PropTypes = React.PropTypes;

var UbuntuScrollView = requireNativeComponent('RCTScrollView');

var ScrollView = React.createClass({
  propTypes: {
    atXBeginning: PropTypes.bool,
    atXEnd: PropTypes.bool,
    atYBeginning: PropTypes.bool,
    atYEnd: PropTypes.bool,
    bottomMargin: PropTypes.number,
    boundsBehavior: PropTypes.number,
    contentHeight: PropTypes.number,
    contentItem: PropTypes.object,
    contentWidth: PropTypes.number,
    contentX: PropTypes.number,
    contentY: PropTypes.number,
    dragging: PropTypes.bool,
    draggingHorizontally: PropTypes.bool,
    draggingVertically: PropTypes.bool,
    flickDeceleration: PropTypes.number,
    flickableChildren: PropTypes.array,
    flickableData: PropTypes.array,
    flickableDirection: PropTypes.number,
    flicking: PropTypes.bool,
    flickingHorizontally: PropTypes.bool,
    flickingVertically: PropTypes.bool,
    horizontalVelocity: PropTypes.number,
    interactive: PropTypes.bool,
    leftMargin: PropTypes.number,
    maximumFlickVelocity: PropTypes.number,
    moving: PropTypes.bool,
    movingHorizontally: PropTypes.bool,
    movingVertically: PropTypes.bool,
    originX: PropTypes.number,
    originY: PropTypes.number,
    pixelAligned: PropTypes.bool,
    pressDelay: PropTypes.number,
    rebound: PropTypes.object,
    rightMargin: PropTypes.number,
    topMargin: PropTypes.number,
    verticalVelocity: PropTypes.number,
    visibleArea: PropTypes.object,

    onScroll: PropTypes.func,
  },

  render: function() {
    return (
      <UbuntuScrollView
        {...this.props}
        style={{flex: 1}}
      />
    );
  }
});

module.exports = ScrollView;
