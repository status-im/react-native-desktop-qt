## React-Native and Desktop Setup

`Desktop` platform support can be added through `rnpm-plugin-desktop` to both existing `react-native` projects and new ones.

In order to setup a fresh project, execute these terminal commands:

```
~$ npm install -g react-native-cli
~$ react-native init --version="0.53.3" DesktopSampleApp
~$ cd DesktopSampleApp
```

Clone `react-native-desktop` by running these commands:

```
~/DesktopSampleApp$ cd ..
~$ git clone https://github.com/status-im/react-native-desktop.git
```

To add desktop support, execute the following commands:

```
~$ cd DesktopSampleApp
~/DesktopSampleApp$ npm install ../react-native-desktop/local-cli/rnpm/ --save-dev
~/DesktopSampleApp$ react-native desktop
```

Finally, remove `rnpm-plugin-desktop`, as it is no longer needed:

```
~/DesktopSampleApp$ npm uninstall rnpm-plugin-desktop --save-dev
```

## Starting the App

If you're using macOS, run these commands in separate shells:

```
~/DesktopSampleApp$ npm start
~/DesktopSampleApp$ node node_modules/react-native/ubuntu-server.js
```

Afterwards, execute:
```
~/DesktopSampleApp$ react-native run-desktop
```

(On non-macOS systems, `npm start` and `node ubuntu-server.js` will be executed automatically by the above command)
