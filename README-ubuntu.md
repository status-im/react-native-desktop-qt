
# React Native Desktop - Ubuntu setup.

## Introduction

### Platforms

There is support for applications on Desktop Ubuntu.

### Prerequisites

You should have an Ubuntu 16.04 (Xenial Xerus) installation.

You should have some familiarity with developing on Ubuntu platforms.

## Setup

Before we can go further, git and node, and node's package manager must be
installed.

```
sudo apt-get install -y git nodejs-legacy npm
```

### Setup React Native

To start working on a React Native application, the React Native packages for
ubuntu need to be published to a local package repository.

Clone Desktop repo:
```
~$ mkdir src; cd src
~/src$ git clone https://github.com/status-im/react-native-desktop -b react-native-qt
```
And then follow the instruction in ~/src/react-native-desktop/react-native-cli/README.md

Roughly they are -
```
~/src$ sudo npm install -g sinopia
~/src$ (edit sinopia config for react)
~/src$ sinopia&
~/src$ npm set registry http://localhost:4873/
~/src$ npm adduser --registry http://localhost:4873/
~/src$ cd reactnative-ubuntu/react-native-cli; npm publish --force
~/src/reactnative-ubuntu/react-native-cli$ cd ..; npm publish --force
```

### Install React Native CLI

The last step in preparing for creating our application to install the CLI
tools from the local package repository.
```
~$ sudo npm install -g react-native-cli
```

## Creating a new React Native application

Now that React Native has been installed we can get started making an
application.

```
~$ cd ~/src
~/src$ react-native init AwesomeProject
~/src$ cd AwesomeProject
~/src/AwesomeProject$ react-native ubuntu
~/src/AwesomeProject$ cd ubuntu
```

That's it, the skeleton of an application is in place. You can edit
~/src/AwesomeProject/index.ubuntu.js to start developing.

## Easy path

The quick and easy way to get started is to use the react-native CLI tool to
build and run the application.
```
~/src/AwesomeProject$ react-native run-ubuntu
```

#### Dependencies

Add Qt 5.9.1 PPA for Ubuntu Trusty:

```
sudo add-apt-repository -y ppa:beineri/opt-qt591-trusty
```

Add Qt 5.9.1 PPA for Ubuntu Xenial:

```
sudo add-apt-repository -y ppa:beineri/opt-qt591-xenial
```

Installing:

```
~$ sudo apt-get install -y cmake qt59base qt59graphicaleffects qt59quickcontrols2 qt59declarative
```

#### Building

Run the build script.
```
~/src/AwesomeProject/ubuntu$ ./build.sh
```

#### Running

In a separate shell, start the React Native packager.
```
~/src/AwesomeProject$ npm start
```

Then it is all good to go. Live reload can be activated by passing the
--live-reload flag.
```
~/src/AwesomeProject/ubuntu$ ./run-app.sh —live-reload
```

## Developing React Native Desktop

If you would like to contribute, or just want a short cut to trying some React
Native examples, you can build React Native directly.

#### Dependencies

You do not need to add react-native and react-native-cli to the
local repository if you are building this way.

#### Building

```
~/src/reactnative-ubuntu$ mkdir build; cd build; cmake .. && make -j4
```

#### Running

In a separate shell, start the React Native packager.
```
~/src/reactnative-ubuntu$ npm start
```

The examples all follow a similar template, e.g to run the TicTacToe app
```
~/src/reactnative-ubuntu$ ./build/Examples/TicTacToe/run-example.sh
```
To run the 2048 app
```
~/src/reactnative-ubuntu$ ./build/Examples/2048/run-example.sh
```

