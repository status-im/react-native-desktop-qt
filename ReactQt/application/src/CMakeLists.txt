# Copyright (C) 2016, Canonical Ltd.
# All rights reserved.

# This source code is licensed under the BSD-style license found in the
# LICENSE file in the root directory of this source tree. An additional grant
# of patent rights can be found in the PATENTS file in the same directory.

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

find_package(Qt5Core REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(Qt5WebSockets REQUIRED)

# Format EXTERNAL_MODULES to contain array of external modules type names defined as strings
string (REPLACE ";" "," EXTERNAL_MODULES "${REACT_NATIVE_DESKTOP_EXTERNAL_MODULES_TYPE_NAMES}")

if (JS_BUNDLE_PATH)
  set(JS_BUNDLE_RESOURCE "<file alias=\"index.desktop.bundle\">${JS_BUNDLE_PATH}</file>")
endif()

configure_file(
  main.qml.in
  ${CMAKE_CURRENT_SOURCE_DIR}/main.qml
)

configure_file(
  main.qrc.in
  ${CMAKE_CURRENT_SOURCE_DIR}/main.qrc
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../runtime/src)

add_executable(
  ${APP_NAME}
  main.cpp
  main.qrc
)

set(USED_QT_MODULES Core Qml Quick WebSockets)

qt5_use_modules(${APP_NAME} ${USED_QT_MODULES})

target_link_libraries(
  ${APP_NAME}
  react-native
)


