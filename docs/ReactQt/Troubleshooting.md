
These are some common issues you may run into while setting up React Native Qt.

### Unrecognized platform: desktop

Error appears when you do `npm-start` in react-native directory and then run application.

Because react-native packager module was moved into separate node package along with separate git repo, to run application it's required manually edit {clone_root}/node_modules/metro-bundler/src/defaults.js by adding `desktop` platform into the list, so it should look like:

`exports.platforms = ['ios', 'android', 'windows', 'web', 'desktop'];`
