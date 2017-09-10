
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

set(REACT_TESTCASE_JS
  JS/TestImageProps.js
  JS/TestActivityIndicatorProps.js
)

set(
  REACT_TESTCASE_SRC
  reacttestcase.cpp
)

set(
  REACT_TESTCASE_LIBRARIES
  Qt5::Test Qt5::Quick react-native
)


add_executable(test-integration test-integration.cpp resources.qrc ${REACT_TESTCASE_SRC})
add_executable(test-image-props test-image-props.cpp resources.qrc ${REACT_TESTCASE_SRC} ${REACT_TESTCASE_JS})
add_executable(test-activityindicator-props test-activityindicator-props.cpp resources.qrc ${REACT_TESTCASE_SRC} ${REACT_TESTCASE_JS})

add_test(test-integration test-integration)
add_test(test-image-props test-image-props)
add_test(test-activityindicator-props test-activityindicator-props)

target_link_libraries(test-integration ${REACT_TESTCASE_LIBRARIES})
target_link_libraries(test-image-props ${REACT_TESTCASE_LIBRARIES})
target_link_libraries(test-activityindicator-props ${REACT_TESTCASE_LIBRARIES})

set_target_properties(
  test-integration
  PROPERTIES LIBRARY_OUTPUT_DIRECTORY tests
)
