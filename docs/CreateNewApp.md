## Overview

In order to setup the RN-desktop project, execute these terminal commands:

```sh
npm install -g react-native-cli # Or use Yarn
react-native init DesktopSampleApp --version status-im/react-native-desktop
cd DesktopSampleApp
react-native desktop
```

If you're using macOS, run these commands in separate shells:
```sh
npm start
node node_modules/react-native/ubuntu-server.js
```

Afterwards, execute:
```sh
react-native run-desktop
```
(On non-macOS systems, `npm start` and `node ubuntu-server.js` will be executed automatically by the above command)
