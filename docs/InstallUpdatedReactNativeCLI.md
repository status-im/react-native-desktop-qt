## Overview

Original `react-native-cli` is extended with CLI commands to generate, build and run react-native projects for `desktop` platform. To be able to create react-native apps for `desktop` platform you should either specify `--version` in the original CLI when creating a react-native app, `react-native init DesktopApp --version status-im/react-native-desktop`, or install the extended version of the CLI by following the next steps and then running `react-native init DesktopApp`.

1. `git clone https://github.com/status-im/react-native-desktop.git`
2. `cd react-native-desktop/react-native-cli`
3. `npm update`
4. `npm uninstall -g react-native-cli` (remove original package if installed)
5. `npm install -g`
