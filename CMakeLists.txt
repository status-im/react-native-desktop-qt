
# Copyright (C) 2016, Canonical Ltd.
# All rights reserved.

# This source code is licensed under the BSD-style license found in the
# LICENSE file in the root directory of this source tree. An additional grant
# of patent rights can be found in the PATENTS file in the same directory.

cmake_minimum_required(VERSION 3.2.2)

project(react-native-desktop)

set(PROJECT_NAME "react-native-desktop")
set(PROJECT_VERSION "0.0.1")

option(JAVASCRIPTCORE_ENABLED
    "Build with JavaScriptCore enabled"
    OFF)

if(JAVASCRIPTCORE_ENABLED)
  set(CMAKE_CXX_STANDARD 14)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
endif()

if(WIN32)
  set(REACT_BUILD_STATIC_LIB 1)
  set(USE_QTWEBKIT 1)
endif()

include_directories(./React/Layout)
enable_testing()

message(STATUS "External modules dirs: ${EXTERNAL_MODULES_DIR}")

foreach(module_directory ${EXTERNAL_MODULES_DIR})
  message(STATUS "Add external module subdirectory: ${module_directory}")
  add_subdirectory(${module_directory})
endforeach(loop_var)

add_subdirectory(ReactQt)
add_subdirectory(Examples)
add_subdirectory(RNTester)

add_custom_target(
  "watch"
  COMMAND node -e \"require('fs').watch('${CMAKE_SOURCE_DIR}/ReactQt',function(e,f){require('child_process').spawn('make',{stdio:'inherit'})})\"
  DEPENDS react-native
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
