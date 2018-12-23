These are some common issues you may run into while setting up React Native Qt.

### `npm install` hangs
Downgrade to version 5.5.1: `npm install -g npm@5.5.1`.

### `react-native run desktop` complaining about missing `qmldir`:

```
Command failed: ./build.sh -e "node_modules/react-native-i18n/desktop;node_modules/react-native-config/desktop;node_modules/react-native-fs/desktop;node_modules/react-native-http-bridge/desktop;node_modules/react-native-webview-bridge/desktop;modules/react-native-status/desktop"
Error copying directory from "/path-to-status-react/node_modules/react-native/ReactQt/runtime/src/qmldir" to "/path-to-status-react/desktop/lib/React".
make[2]: *** [lib/CMakeFiles/copy-qmldir] Error 1
make[1]: *** [lib/CMakeFiles/copy-qmldir.dir/all] Error 2
make: *** [all] Error 2
```

Can be solved by re-running `npm install react-native` which put the `ReactQt/runtime/src/qmldir` file back.

### Missing web3 package issue

After last upgrade of react-native-desktop to the v.0.53.3 of original react-native appeared some incompatibility between `react-native` and `web3` packages on npm install. Initially it installed usually fine, but after `react-native desktop` command execution `web3` package is get removed from `node_modules`. Manual install of web3 by `npm install web3` installs `web3` package, but removes `react-native` package. Workaround or solution?

### ReactButton.qml non-existent property "elide" error upon startup

```
qrc:/qml/ReactButton.qml:33: Error: Cannot assign to non-existent property "elide"
"Component for qrc:/qml/ReactWebView.qml is not loaded"
QQmlComponent: Component is not ready
"Unable to construct item from component qrc:/qml/ReactWebView.qml"
"Can't create QML item for componenet qrc:/qml/ReactWebView.qml"
"RCTWebViewView" has no view for inspecting!
```

Solution: make sure that QtWebEngine components are installed.

### Could not find a package configuration file provided by "Qt5WebKit"
For Mac and Linux `react-native-desktop` uses Qt WebEngine module to implement `WebView` component.

Ufortunately Qt doesn't support Qt WebEngine on Windows with MinGW, only with MSVC. And so far `react-native-desktop` supports only MinGW builds.

To overcome this problem on Windows we have `WebView` implemented with QTWebKit. But this module is outdated and not installed with Qt by default.
Currently there is [issue opened](https://github.com/status-im/react-native-desktop/issues/222) to support MSVC on Windows. That will eventually fix the problem.

Until MSVC supported you have 2 options:
- Build QT5WebKit locally. Process described here - https://github.com/MaxRis/webkit/wiki/Building-QtWebKit-on-Windows-(Conan)
- Disable dependency on Qt5WebKit by changing this CMake option - https://github.com/status-im/react-native-desktop/blob/master/CMakeLists.txt#L28 (probably, you will need manually to clean up all generated CMake cache files to allow new option value to be applied correctly). You wont't be able to use `WebView`, but all other components will work.
