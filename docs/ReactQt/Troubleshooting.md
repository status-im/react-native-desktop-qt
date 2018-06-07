These are some common issues you may run into while setting up React Native Qt.

### `npm install` hangs
Downgrade to version 5.5.1: `npm install -g npm@5.5.1`.

### `react-native run desktop` complaining about missing `qmldir`:
```Command failed: ./build.sh -e "node_modules/react-native-i18n/desktop;node_modules/react-native-config/desktop;node_modules/react-native-fs/desktop;node_modules/react-native-http-bridge/desktop;node_modules/react-native-webview-bridge/desktop;modules/react-native-status/desktop"
Error copying directory from "/path-to-status-react/node_modules/react-native/ReactQt/runtime/src/qmldir" to "/path-to-status-react/desktop/lib/React".
make[2]: *** [lib/CMakeFiles/copy-qmldir] Error 1
make[1]: *** [lib/CMakeFiles/copy-qmldir.dir/all] Error 2
make: *** [all] Error 2
```
Can be solved by re-running `npm install react-native` which put the `ReactQt/runtime/src/qmldir` file back.

### Missing web3 package issue

After last upgrade of react-native-desktop to the v.0.53.3 of original react-native appeared some incompatibility between `react-native` and `web3` packages on npm install. Initially it installed usually fine, but after `react-native desktop` command execution `web3` package is get removed from `node_modules`. Manual install of web3 by `npm install web3` installs `web3` package, but removes `react-native` package. Workaround or solution?

### Node server crashing
`node ./ubuntu_server.js` log:
```
DEBUG [status-im.utils.handlers:36] - Handling re-frame event:  :signal-event {"type":"node.crashed","event":{"error":"node is already running"}}
DEBUG [status-im.ui.screens.events:350] - :event-str {"type":"node.crashed","event":{"error":"node is already running"}}
DEBUG [status-im.utils.instabug:8] - Signal event: {"type":"node.crashed","event":{"error":"node is already running"}}
DEBUG [status-im.ui.screens.events:362] - Event  node.crashed  not handled
```
Solution: prevent starting Node when there is an instance already running.

### ReactButton.qml non-existent property "elide" error upon startup
```
qrc:/qml/ReactButton.qml:33: Error: Cannot assign to non-existent property "elide"
"Component for qrc:/qml/ReactWebView.qml is not loaded"
QQmlComponent: Component is not ready
"Unable to construct item from component qrc:/qml/ReactWebView.qml"
"Can't create QML item for componenet qrc:/qml/ReactWebView.qml"
"RCTWebViewView" has no view for inspecting!
```
Make sure that QtWebEngine is installed.
