
# Copyright (c) 2017-present, Status Research and Development GmbH.
# All rights reserved.

# This source code is licensed under the BSD-style license found in the
# LICENSE file in the root directory of this source tree. An additional grant
# of patent rights can be found in the PATENTS file in the same directory.

project(tests)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

find_package(Qt5Test REQUIRED)
find_package(Qt5Quick REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/common)

set(
  REACT_TEST_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/common/reacttestcase.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/common/reactpropertytestcase.cpp
)

set(
  REACT_TESTCASE_LIBRARIES
  Qt5::Test Qt5::Quick react-native
)

add_subdirectory(test-image-props)
add_subdirectory(test-activityindicator-props)
add_subdirectory(test-button-props)
add_subdirectory(test-array-reconciliation)
add_subdirectory(test-button-size)
add_subdirectory(test-modal-props)
add_subdirectory(test-netexecutor-socket)
add_subdirectory(test-picker-props)
add_subdirectory(test-slider-props)
add_subdirectory(test-textinput-clear)
add_subdirectory(test-textinput-props )



#add_executable(test-integration test-integration.cpp resources.qrc ${REACT_TESTCASE_SRC})
#add_test(test-integration test-integration)
#target_link_libraries(test-integration ${REACT_TESTCASE_LIBRARIES})
#set_target_properties(
#  test-integration
#  PROPERTIES LIBRARY_OUTPUT_DIRECTORY tests
#)
