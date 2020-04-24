
# Copyright (c) 2017-present, Status Research and Development GmbH.
# All rights reserved.

# This source code is licensed under the BSD-style license found in the
# LICENSE file in the root directory of this source tree. An additional grant
# of patent rights can be found in the PATENTS file in the same directory.

set(TEST_NAME test-activityindicator-props)

set(REACT_TESTCASE_JS
  TestActivityIndicatorProps.js
)


add_executable(${TEST_NAME} ${TEST_NAME}.cpp resources.qrc ${REACT_TEST_SOURCES} ${REACT_TESTCASE_JS})
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
target_link_libraries(${TEST_NAME} ${REACT_TESTCASE_LIBRARIES})
