# [React Native Desktop](https://github.com/status-im/react-native-desktop) &middot; [![Circle CI Status](https://circleci.com/gh/status-im/react-native-desktop.svg?style=shield)](https://circleci.com/gh/status-im/react-native-desktop) [![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg)](https://github.com/status-im/react-native-desktop/issues)


Cross-platform React Native Desktop port based on Qt framework.

Supported operating systems:
- Linux (checked with Ubuntu >= 16.04 LTS)
- Mac OS (checked with Mac OS X versions)
- Windows


<!-- ## Table of contents
- [Getting Started](#getting-started)
- [Documentation](#documentation)
- [License](#license) -->

## Docs update TODO:
- Add doc on how to run RNTester
- elaborate debugging

# Documentation
## Getting started

- Install [Prerequisites](docs/InstallPrerequisites.md)
- Take a look at [list of supported React Native components and APIs](docs/ComponentsSupport.md)
- [Run RNTester](docs/RunRNTester.md) to examine controls and APIs TODO!!


## Creating your own app
- [Create new App](docs/CreateNewApp.md)
- Learn about [3rd-party react-native modules support](docs/NativeModulesSupport.md)
- [Troubleshooting](docs/Troubleshooting.md)
- [FAQ](#docs/FAQ.md)


### Debugging react-native-desktop apps

To access `In-App Developer Menu` ~~start shaking your laptop/PC~~ press CTRL+R.  
`In-App Developer Menu` is available in Debug builds.
- [Using GammaRay for inspecting Qt internals](docs/InspectAppWithGammaRay.md)


## Contributing
Any kind of contribution is welcome! Check the [list of opened issues](https://github.com/status-im/react-native-desktop/issues) or create new one.

Docs you may find helpful:

- [How react-native-desktop works internally](docs/HowRNDesktopAppWorks.md)
- [Yoga layout engine in react-native-desktop](docs/YogaLayoutEngine.md)
- [Environment setup guide for contributors](docs/ContributorEnvSetup.md)
- [Merging React Native](#docs/MergingReactNative.md)


## ClojureScript React Native apps support

[Re-Natal fork](https://github.com/status-im/re-natal.git) extended with `desktop` platform support.


## License

React Native Desktop is provided under [BSD licensed](./LICENSE).
