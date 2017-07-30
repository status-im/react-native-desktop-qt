
# Copyright (C) 2016, Canonical Ltd.
# All rights reserved.

# This source code is licensed under the BSD-style license found in the
# LICENSE file in the root directory of this source tree. An additional grant
# of patent rights can be found in the PATENTS file in the same directory.

cmake_minimum_required(VERSION 2.8.11)

project(react-native-ubuntu)

set(PROJECT_NAME "react-native-ubuntu")
set(PROJECT_VERSION "0.0.1")

include_directories(./React/Layout)

add_subdirectory(ReactQt)
add_subdirectory(Examples)

add_custom_target(
  "watch"
  COMMAND node -e \"require('fs').watch('${CMAKE_SOURCE_DIR}/ReactQt',function(e,f){require('child_process').spawn('make',{stdio:'inherit'})})\"
  DEPENDS react-native
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
