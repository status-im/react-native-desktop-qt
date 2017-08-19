
# Copyright (c) 2017-present, Status Research and Development GmbH.
# All rights reserved.

# This source code is licensed under the BSD-style license found in the
# LICENSE file in the root directory of this source tree. An additional grant
# of patent rights can be found in the PATENTS file in the same directory.

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

find_package(Qt5Test REQUIRED)
find_package(Qt5Quick REQUIRED)

add_executable(test-integration test-integration.cpp resources.qrc)
add_test(test-integration test-integration)

target_link_libraries(test-integration Qt5::Test Qt5::Quick react-native)

set_target_properties(
  test-integration
  PROPERTIES LIBRARY_OUTPUT_DIRECTORY tests
)
