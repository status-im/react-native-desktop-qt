# Build, develop and debug native Qt modules of react-native-desktop with Qt Creator

### Prerequisites

You should have installed `cmake` and `Qt 5.9.1`.

Qt 5.9.1 PPA for Ubuntu Trusty:

```
sudo add-apt-repository -y ppa:beineri/opt-qt591-trusty
```

Qt 5.9.1 PPA for Ubuntu Xenial:

```
sudo add-apt-repository -y ppa:beineri/opt-qt591-xenial
```

Install:

```
~$ sudo apt-get install -y cmake qt59base qt59graphicaleffects qt59quickcontrols2 qt59declarative
```

`Qt Creator` IDE should be installed.

### Qt Creator cmake toolchain configuration

`Qt Creator` should be configured with path to CMake tool.  
Launch `Qt Creator`, select `Tools`->`Options..`->`Build & Run`->`CMake` tab.
Make sure that `CMake` tool is auto-detected by `Qt Creator` or add one manually.

It may be necessary to add:

```
set(CMAKE_PREFIX_PATH "<path_to_Qt>/5.9.1/gcc_64/lib/cmake")
```

to the root `CMakeLists.txt` file in `react-native-desktop` repo in order for QtCreator to find the necessary packages.

### Build react-native-desktop with Qt Creator

In `Qt Creator` select menu `File`->`Open File or Project...`.  
Navigate to the root directory of your clone and select `CMakeList.txt` file to open.

Select Qt 5.9.1 toolchain to configure the project, if prompted.

By default, Qt configures `CMake` to do out-of-directory build on one level higher than your repo clone directory.

`Qt Creator` provides settings for `Build` and `Run` at the left side.

Select `Build` and under `Build Settings` select desired configuration - `Debug`, `Release` and others.  
To launch debug session we are interested in `Debug` configuration.  
Select `Debug` and start run of `CMake` tool by selecting `Build`->`Run CMake`.  

If `CMake` run finished without error, you can start the build `Build`->`Build All`. Check `Compile Output` tab for compilation results.

### Debugging and running of Examples with Qt Creator

Below we will run TicTacToe example.

Select `Build & Run`->`Run`. Create configuration to run `Example` executable.  
Place value `./Examples/TicTacToe/run-example.sh` into `Executable` field.
Select `Build`->`Run` to run TicTacToe example application.

To use debugger for step by step C++ code debugging, create new `Run` configuration.  
Fill `Executable` field with value `qmlscene`.
Fill `Command line arguments` field with value `qmlscene -I ./ReactQt/runtime/src/ ./Examples/TicTacToe/TicTacToe.qml`.
Select `Debug`->`Start Debugging`->`Start Debugging`.
Debugger will be attached to started `qmlscene` process and you will able to break on your breakpoints in C++ source code.

You will need to update the paths to run others examples.
