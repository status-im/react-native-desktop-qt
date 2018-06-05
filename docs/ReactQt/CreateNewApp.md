## Overview

To create react-native apps with `Desktop` platform support extended version of `react-native-cli` package should be [installed globally](https://github.com/status-im/react-native-desktop/blob/master/docs/ReactQt/InstallUpdatedReactNativeCLI.md).  

In order to setup the RN-desktop project, execute these terminal commands:

```
~$ react-native init DesktopSampleApp && cd DesktopSampleApp
~/DesktopSampleApp$ react-native desktop

```
Afterwards run these commands in separate shells:
```
~/DesktopSampleApp$ npm start
~/DesktopSampleApp$ node node_modules/react-native/ubuntu-server.js
~/DesktopSampleApp$ react-native run-desktop
```

