
# Copyright (C) 2016, Canonical Ltd.
# All rights reserved.

# This source code is licensed under the BSD-style license found in the
# LICENSE file in the root directory of this source tree. An additional grant
# of patent rights can be found in the PATENTS file in the same directory.

cmake_minimum_required(VERSION 2.8.11)

option(JAVASCRIPTCORE_ENABLED
    "Build with JavaScriptCore enabled"
    OFF)

if(JAVASCRIPTCORE_ENABLED)
  set(CMAKE_CXX_STANDARD 14)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
endif()

set(APP_NAME <%=name%>)
set(JS_APP_NAME <%=name%>)
set(REACT_BUILD_STATIC_LIB ON)

if (WIN32)
  set(USE_QTWEBKIT 1)
endif()

message(STATUS "EXTERNAL_MODULES_DIR: ${EXTERNAL_MODULES_DIR}")

foreach(external_module ${EXTERNAL_MODULES_DIR})
  message(STATUS "external_module: ${external_module}")
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../${external_module} ${CMAKE_CURRENT_BINARY_DIR}/${external_module})
endforeach(external_module)

# APPLICATION_MAIN_CPP_PATH contains absolute path to generated template copy of main.cpp for application executable
get_filename_component(APPLICATION_MAIN_CPP_PATH main.cpp ABSOLUTE)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../node_modules/react-native-desktop-qt/React/Layout)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../node_modules/react-native-desktop-qt/ReactQt/runtime/src ${CMAKE_CURRENT_BINARY_DIR}/lib)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../node_modules/react-native-desktop-qt/ReactQt/application/src ${CMAKE_CURRENT_BINARY_DIR}/bin)

if (WIN32)
  set(RUN_SCRIPT_FILE_NAME "run-app.bat")
else()
  set(RUN_SCRIPT_FILE_NAME "run-app.sh")
endif()

configure_file(
  ${RUN_SCRIPT_FILE_NAME}.in
  ${CMAKE_CURRENT_BINARY_DIR}/${RUN_SCRIPT_FILE_NAME}
  @ONLY
)
