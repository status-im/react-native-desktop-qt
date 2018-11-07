# [React Native Desktop](https://github.com/status-im/react-native-desktop) &middot; [![Circle CI Status](https://circleci.com/gh/status-im/react-native-desktop.svg?style=shield)](https://circleci.com/gh/status-im/react-native-desktop) [![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg)](https://github.com/status-im/react-native-desktop/issues)


Cross-platform React Native Desktop port based on Qt framework.

Supported operating systems:
- Linux (checked with Ubuntu >= 16.04 LTS)
- Mac OS (checked with Mac OS X versions)
- Windows


>↓↓↓ Move to prerequisites

Recommended Qt framework of version 5.9.1 LTS.


<!-- ## Table of contents
- [Getting Started](#getting-started)
- [Documentation](#documentation)
- [License](#license) -->

## Docs udpate TODO:
- Change 5.9.1 -> 5.11.1
- Get rid of `ubuntu` in filenames
- More details on how to run Examples
- Add doc on how to run RNTester
- Add doc on code formatting

# Documentation
## Getting started

- Install [Prerequisites](docs/InstallPrerequisites.md)
- Take a look at [list of supported React Native components and APIs](docs/ComponentsSupport.md)
- [Run RNTester](docs/RunRNTester.md) to examine controls and APIs TODO!!


## Creating your own app
- [Create new App](docs/CreateNewApp.md)
- Learn about [3rd-party react-native modules support](docs/NativeModulesSupport.md)
- [Installing adjusted react-native-cli](docs/InstallUpdatedReactNativeCLI.md)
- Follow [Ubuntu development guide](README-ubuntu.md) to get started building of react-native-desktop itself and JS apps based on it.
- [Troubleshooting](docs/Troubleshooting.md)
- [FAQ](#docs/FAQ.md)


### Debugging react-native-desktop apps

To access `In-App Developer Menu` ~~start shaking your laptop/PC~~ press CTRL+R.  
`In-App Developer Menu` is available in Debug builds.
- [Using GammaRay for inspecting Qt internals](docs/InspectAppWithGammaRay.md)


## Contributing
Any kind of contribution is welcome! Check the [list of opened issues](https://github.com/status-im/react-native-desktop/issues) or create new one.

- [Yoga layout engine in react-native-desktop](docs/YogaLayoutEngine.md)
- [How react-native-desktop works internally](docs/HowRNDesktopAppWorks.md)
- [Linux guide for React Native Desktop contributors](Development-linux.md)
- [Cpp code formatting](docs/CodeFormatting.md) TODO!!
- [Merging React Native](#docs/MergingReactNative.md)


## ClojureScript React Native apps support

[Re-Natal fork](https://github.com/status-im/re-natal.git) extended with `desktop` platform support.


## License

React Native Desktop is provided under [BSD licensed](./LICENSE).
