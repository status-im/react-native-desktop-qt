## Overview

`react-native-desktop` depends on cross-platform development tools, so prerequisites are pretty similar for all supported platforms. In nutshell, toolchain is the same as for original `react-native` + native dev tools (Qt framework) + build system based on `cmake`.

### Windows

Recommended way to install required tools and libs is to use [Chocolatey](https://chocolatey.org/) package manager.

```
choco install -y nodejs.install python2 cmake git
```

Download and run [Qt 5.9.1](https://download.qt.io/archive/qt/5.9/5.9.1/qt-opensource-windows-x86-5.9.1.exe) installer for Windows. During installation process on `Select Components` screen check `Qt 5.9.1 / MinGW 5.3.0 32 bit`, `Qt 5.9.1 / Qt WebEngine` and `Tools / MinGW 5.3.0` components. Finish installation process.

Make sure that installed Qt's `qmake` tool is available in your PATH.

### Mac OS

Recommended way to install required tools and libs is to use [Homebrew](https://brew.sh/) package manager.

```
brew install node watchman cmake
```

Download and run [Qt 5.9.1](https://download.qt.io/archive/qt/5.9/5.9.1/qt-opensource-mac-x64-5.9.1.dmg) installer for Mac OS. During installation process on `Select Components` screen check `Qt 5.9.1 / macOS` and `Qt 5.9.1 / Qt WebEngine` components. Finish installation process.

Make sure that installed Qt's `qmake` tool is available in your PATH.

### Ubuntu

```
sudo apt-get install -y git cmake nodejs-legacy npm
```

Download and run [Qt 5.9.1](https://download.qt.io/archive/qt/5.9/5.9.1/qt-opensource-linux-x64-5.9.1.run) installer for Linux. Finish installation process.

Make sure that installed Qt's `qmake` tool is available in your PATH.