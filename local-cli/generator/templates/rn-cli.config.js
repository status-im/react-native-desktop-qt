'use strict';

var config = {
  getPlatforms() {
    return ['android', 'ios', 'desktop'];
  },
  getProvidesModuleNodeModules() {
    return ['react-native', 'react-native-desktop'];
  }
};

module.exports = config;