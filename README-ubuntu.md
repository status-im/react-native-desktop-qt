
# React Native Ubuntu

## Introduction

### Platforms

There is support for applications on both Desktop Ubuntu and Ubuntu Touch.

### Prerequisites

You should have an Ubuntu 16.04 (Xenial Xerus) installation.

If you are going to be developing on a device:
 - You must have a device with Ubuntu Touch installed and developer mode
 enabled, see [Touch installation page](https://developer.ubuntu.com/en/phone/devices/installing-ubuntu-for-devices/)
 for more details.
 - Your device must be on the same local network as your development machine.

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

Download the port for Ubuntu.
```
~$ mkdir src; cd src
~/src$ git clone https://github.com/CanonicalLtd/react-native -b ubuntu
```
And then follow the instruction in ~/src/react-native-linux/react-native-cli/README.md

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

### Setup node-haste

Some changes to node-haste are required to detect the Ubuntu platform. Clone
the patched repo and publish to the local package repository.
```
~/src$ git clone https://github.com/justinmcp/node-haste
~/src$ cd node-haste; npm install
~/src/node-haste$ (add node-haste to sinopia config - see below)
~/src/node-haste$ npm publish --force
```

node-haste should be added to the packages section of the Sinopia config file
```
    packages:
      'node-haste':
        allow_access: $all
        allow_publish: $all
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

If you have a connected Ubuntu device, you can launch onto the device with
```
~/src/AwesomeProject$ react-native run-ubuntu --arch=arm
```
*Note*; For device builds, the Ubuntu SDK must have been installed as per the
instructions below.


Other command line arguments can be passed 
```
~/src/AwesomeProject$ react-native run-ubuntu -h
```
To be able to read the list.


### Targeting a device

#### Dependencies

Usually we would want to be installing the ubuntu-sdk, which can be done
directly, but since this is still in preview, we will just install a raw build
environment. The build environment is a chroot provided by the hardworking SDK
team, but is not part of the general distribution. We will need to add the SDK
team's repository first.

```
~$ sudo add-apt-repository -y ppa:ubuntu-sdk-team/ppa
~$ sudo apt-get update && sudo apt-get upgrade -y
```

Then we can install all necessary packages.
```
~$ sudo apt-get install -y adb click-dev ubuntu-sdk-api-15.04-armhf
```


#### Building

To build for the device, run the build script through the chroot build
environment.
```
~/src/AwesomeProject/ubuntu$ click chroot -a armhf -f ubuntu-sdk-15.04 -n click run ./build.sh
```

#### Running

Make sure that the device can be connected to via adb. The device will need to
have developer mode enabled, instruction on these topics can be found on the
[Touch installation page](https://developer.ubuntu.com/en/phone/devices/installing-ubuntu-for-devices/).

In a separate shell, start the React Native packager.
```
~/src/AwesomeProject$ npm start
```

Then it is all good to go. Live reload can be activated by passing the --live-reload flag.
```
~/src/AwesomeProject/ubuntu$ ./run-app.sh —on-device —live-reload
```

### Targeting a desktop

#### Dependencies

```
~$ sudo apt-get install -y cmake qt5-default qtdeclarative5-dev
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

## Developing React Native Ubuntu

If you would like to contribute, or just want a short cut to trying some React
Native examples, you can build React Native directly.

#### Dependencies

```
~$ sudo apt-get install -y cmake qt5-default qtdeclarative5-dev
~/src/reactnative-ubuntu$ npm install
```

*Note*; you do not need need to add react-native and react-native-cli to the
local repository if you are building this way, but you must add node-haste, as
per the instructions above.

#### Building

```
~/src/reactnative-ubuntu$ mkdir build; cd build; cmake .. && make
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

