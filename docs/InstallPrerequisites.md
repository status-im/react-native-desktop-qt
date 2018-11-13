## Overview

`react-native-desktop` depends on cross-platform development tools, so prerequisites are pretty similar for all supported platforms. In nutshell, toolchain is the same as for original `react-native` + native dev tools (Qt framework) + build system based on `cmake`.

### Windows

Recommended way to install required tools and libs is to use [Chocolatey](https://chocolatey.org/) package manager.

```
choco install -y nodejs.install python2 cmake git
```

Download and run [Qt 5.11.2](https://download.qt.io/archive/qt/5.11/5.11.2/qt-opensource-windows-x86-5.11.2.exe) installer for Windows. During installation process on `Select Components` screen check `Qt 5.11.2 / MinGW 5.3.0 32 bit`, `Qt 5.11.2 / Qt WebEngine` and `Tools / MinGW 5.3.0` components. Finish installation process.

### Mac OS

Recommended way to install required tools and libs is to use [Homebrew](https://brew.sh/) package manager.

```
brew install node watchman cmake
```

Download and run [Qt 5.11.2](https://download.qt.io/archive/qt/5.11/5.11.2/qt-opensource-mac-x64-5.11.2.dmg) installer for Mac OS. During installation process on `Select Components` screen check `Qt 5.11.2 / macOS` and `Qt 5.11.2 / Qt WebEngine` components. Finish installation process.

### Ubuntu

```
sudo apt-get install -y git cmake nodejs-legacy npm
```

Download and run [Qt 5.11.2](https://download.qt.io/archive/qt/5.11/5.11.2/qt-opensource-linux-x64-5.11.2.run) installer for Linux. Finish installation process.

### All platforms

Make sure that installed Qt's `qmake` tool is available in your PATH.
On mac and linux you can add following to `.bash_profile`:

`export PATH=$PATH:/Users/MyUser/Qt/5.11.2/COMPILER_NAME/bin`
