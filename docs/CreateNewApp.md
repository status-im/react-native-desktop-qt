:warning::warning::warning:
#### React Native Desktop is under active redesign now. Plese consider documentation as out of date until it is adapted.

---
## Overview

#### Create react native project
```sh
npm install -g react-native-cli
react-native init DesktopSampleApp --version react-native@0.57.8
```

#### Add desktop rnpm plugin
```sh
cd DesktopSampleApp
yarn add git+https://github.com/status-im/rnpm-plugin-desktop-qt.git --dev
```
RNPM plugin gives you access to a new command that generates desktop files for your project.


#### Generate desktop files
```sh
react-native desktop-qt
```
This command will add `react-native-desktop-qt` package to your project and generate `desktop` folder with desktop project.


#### Run the project
If you're using macOS, run these commands in 2 different shells (from `DesktopSampleApp` dir):
```sh
npm start #starts bundler
```
```sh
node node_modules/react-native/js-executor.js #starts js server
```

Afterwards, in a 3rd shell execute:
```sh
react-native run-desktop
```
Compilation of desktop project will start. Then it will run:

![](./media/react-native-desktop-new-app.png)

**Note:** On non-macOS systems, `npm start` and `node js-executor.js` will be executed automatically by the above command(`react-native run-desktop`)


**If you want to know why you had to start bundler and js server** - check the doc on [how react-native-desktop works internally](./HowRNDesktopAppWorks.md)
