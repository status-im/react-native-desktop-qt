
# Copyright (C) 2016, Canonical Ltd.
# All rights reserved.

# This source code is licensed under the BSD-style license found in the
# LICENSE file in the root directory of this source tree. An additional grant
# of patent rights can be found in the PATENTS file in the same directory.

cmake_minimum_required(VERSION 2.8.11)

set(APP_NAME <%= name %>)
set(REACT_BUILD_STATIC_LIB ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../node_modules/react-native/React/Layout)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../node_modules/react-native/ReactQt/runtime/src ${CMAKE_CURRENT_BINARY_DIR}/lib)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../node_modules/react-native/ReactQt/application/src ${CMAKE_CURRENT_BINARY_DIR}/bin)

configure_file(
  run-app.sh.in
  ${CMAKE_CURRENT_BINARY_DIR}/run-app.sh
  @ONLY
)

