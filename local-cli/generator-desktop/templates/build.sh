#!/bin/bash

# Copyright (C) 2016, Canonical Ltd.
# All rights reserved.

# This source code is licensed under the BSD-style license found in the
# LICENSE file in the root directory of this source tree. An additional grant
# of patent rights can be found in the PATENTS file in the same directory.

# XXX: Don't move this script
cd $(dirname $0)
externalModulesPaths="$1"

echo $externalModulesPaths

# Workaround
rm -rf CMakeFiles CMakeCache.txt cmake_install.cmake Makefile

# Build project
cmake -DEXTERNAL_MODULES_DIR="$externalModulesPaths" . && make && cp ./bin/<%= name %> click/


