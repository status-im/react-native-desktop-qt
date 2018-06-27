# [React Native Desktop](https://github.com/status-im/react-native-desktop) &middot; [![Circle CI Status](https://circleci.com/gh/status-im/react-native-desktop.svg?style=shield)](https://circleci.com/gh/status-im/react-native-desktop) [![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg)](https://github.com/status-im/react-native-desktop/issues)

Cross-platform React Native Desktop port based on Qt framework.

Supported operating systems:
- Linux (checked with Ubuntu >= 16.04 LTS)
- Mac OS (checked with Mac OS X versions)
- Windows

Recommended Qt framework of version 5.9.1 LTS.

- [Getting Started](#getting-started)
- [Documentation](#documentation)
- [Contributing](#contributing)
- [License](#license)

## Getting Started

- [Install Prerequisites](docs/InstallPrerequisites.md)
- [Install adjusted react-native-cli](docs/InstallUpdatedReactNativeCLI.md)
- [Create new App](docs/CreateNewApp.md)


### Debugging

To access `In-App Developer Menu` ~~start shaking your laptop/PC~~ press CTRL+R.  
`In-App Developer Menu` is available in Debug builds.

### 3rd-party react-native modules support
Pure JavaScript react-native modules can be used with react-native-desktop without limitations.  
Creation of custom react-native `native` modules or `adopting of available native modules` is supported and described in following [guideline](docs/ReactQt/NativeModulesSupport.md).  

[List of supported or partially supported native modules](docs/ReactQt/SupportedNativeModulesList.md).

## Documentation

- [How react-native-desktop works internally](docs/ReactQt/HowRNDesktopAppWorks.md)
- [List of supported React Native components and APIs ("React Native Qt" column)](docs/ReactQt/ComponentsSupport.md)
- Follow [Ubuntu development guide](README-ubuntu.md) to get started building of react-native-desktop itself and JS apps based on it.
- [Troubleshooting](docs/ReactQt/Troubleshooting.md)

## ClojureScript React Native apps support

[Re-Natal fork](https://github.com/status-im/re-natal.git) extended with `desktop` platform support.

Read our [contributing guide](https://facebook.github.io/react-native/docs/contributing.html) to learn about our development process, how to propose bug fixes and improvements, and how to build and test your changes to React Native.

Any kind of contribution is welcome! Check the [list of opened issues](https://github.com/status-im/react-native-desktop/issues) or create new one.

- [Linux guide for React Native Desktop contributors](Development-linux.md)
- [Yoga layout engine in react-native-desktop](YogaLayoutEngine.md)
- [Using GammaRay for inspecting Qt internals](docs/ReactQt/InspectAppWithGammaRay.md)

## License

React Native Desktop is provided under [BSD licensed](./LICENSE).
