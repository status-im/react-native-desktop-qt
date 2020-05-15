:warning::warning::warning:
#### React Native Desktop is under active redesign now. Plese consider documentation as out of date until it is adapted.

---
## Overview

#### Create react native project
```sh
npx react-native init DesktopSampleApp --version 0.60.6
```

#### Add desktop support by invoking `react-native-desktop-qt-init` package
```sh
cd DesktopSampleApp
npx https://github.com/status-im/react-native-desktop-qt-init.git
```
This command will add `react-native-desktop-qt` package to your project and generate `desktop` folder with desktop project files.
Unless you have local chagnes, answer 'y' when shell prompts you to replace `babel.config.js`


#### Run the project
If you're using macOS, run these commands in 2 different shells (from `DesktopSampleApp` dir):
```sh
npm start #starts bundler
```
```sh
node node_modules/react-native-desktop-qt/js-executor.js #starts js server
```

Afterwards, in a 3rd shell execute:
```sh
npx react-native run-desktop
```
Compilation of desktop project will start. Then it will run:

![](./media/react-native-desktop-new-app.png)

**Note:** On non-macOS systems, `npm start` and `node js-executor.js` will be executed automatically by the above command(`react-native run-desktop`)


**If you want to know why you had to start bundler and js server** - check the doc on [how react-native-desktop works internally](./HowRNDesktopAppWorks.md)
