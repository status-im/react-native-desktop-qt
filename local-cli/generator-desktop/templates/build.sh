#!/bin/bash

# Copyright (C) 2016, Canonical Ltd.
# All rights reserved.

# This source code is licensed under the BSD-style license found in the
# LICENSE file in the root directory of this source tree. An additional grant
# of patent rights can be found in the PATENTS file in the same directory.

# XXX: Don't move this script
cd $(dirname $0)
for i in "$@"
do
case $i in
  -e=*|--externalModulesPaths=*)
  ExternalModulesPaths="${i#*=}" ;;
  -j=*|--jsBundlePath=*)
  JsBundlePath="${i#*=}" ;;
esac
done

echo "build.sh external modules paths: "$ExternalModulesPaths
echo "build.sh JS bundle path: "$JsBundlePath

# Workaround
rm -rf CMakeFiles CMakeCache.txt cmake_install.cmake Makefile

# Build project
cmake -DCMAKE_BUILD_TYPE=Debug -DEXTERNAL_MODULES_DIR="$ExternalModulesPaths" -DJS_BUNDLE_PATH="$JsBundlePath" . && make && cp ./bin/TestProject10 click/
