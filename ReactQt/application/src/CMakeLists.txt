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
set(FONTS_RESOURCE "")

find_package(Qt5Core REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(Qt5WebSockets REQUIRED)
find_package(Qt5Svg REQUIRED)

if (USE_QTWEBKIT)
  find_package(Qt5WebKit REQUIRED)
endif()

# Add all external desktop fonts to resources. Fonts will be outside of directory with qrc,
# so we are using aliases to load them
foreach(FONT_PATH ${DESKTOP_FONTS})
    get_filename_component(FONT_FILENAME ${FONT_PATH} NAME)
    STRING(CONCAT TMP ${FONTS_RESOURCE} "<file alias=\"${FONT_FILENAME}\">${FONT_PATH}</file>")
    SET(FONTS_RESOURCE ${TMP})
endforeach(FONT_PATH)


# Format EXTERNAL_MODULES to contain array of external modules type names defined as strings
string (REPLACE ";" "," EXTERNAL_MODULES "${REACT_NATIVE_DESKTOP_EXTERNAL_MODULES_TYPE_NAMES}")

if (JS_BUNDLE_PATH)
  set(JS_BUNDLE_RESOURCE "<file alias=\"index.desktop.bundle\">${JS_BUNDLE_PATH}</file>")
endif()
set(ICON_PNG_RESOURCE "<file alias=\"icon.png\">${ICON_PNG_RESOURCE_PATH}</file>")

if (BUILD_FOR_BUNDLE)
  # Set some Win32 Specific Settings
  if (WIN32)
    set(GUI_TYPE WIN32)
  endif(WIN32)
endif(BUILD_FOR_BUNDLE)

configure_file(
  main.qml.in
  ${CMAKE_CURRENT_SOURCE_DIR}/main.qml
)

configure_file(
  main.qrc.in
  ${CMAKE_CURRENT_SOURCE_DIR}/main.qrc
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../runtime/src)

if (REACT_NATIVE_DESKTOP_MAIN_APP_EXTERNAL_MODULES_INCLUDE_DIRS)
  include_directories(${REACT_NATIVE_DESKTOP_MAIN_APP_EXTERNAL_MODULES_INCLUDE_DIRS})
endif()


if (APPLICATION_MAIN_CPP_PATH)
  set(MAIN_CPP_SOURCE ${APPLICATION_MAIN_CPP_PATH})
else()
  set(MAIN_CPP_SOURCE main.cpp)
endif()

if (APPLICATION_MAIN_RC_PATH)
  set(MAIN_RC_PATH ${APPLICATION_MAIN_RC_PATH})
endif()

add_executable(
  ${APP_NAME}
  ${GUI_TYPE}
  ${MAIN_CPP_SOURCE}
  main.qrc
  ${MAIN_RC_PATH}
)

set(USED_QT_MODULES Core Qml Quick WebSockets Svg Concurrent)
if (USE_QTWEBKIT)
  set (USED_QT_MODULES ${USED_QT_MODULES} Webkit)
endif()

qt5_use_modules(${APP_NAME} ${USED_QT_MODULES})

target_link_libraries(
  ${APP_NAME}
  react-native
)

if (REACT_NATIVE_DESKTOP_MAIN_APP_EXTERNAL_PROJECT_DEPS)
  target_link_libraries(${APP_NAME} ${REACT_NATIVE_DESKTOP_MAIN_APP_EXTERNAL_PROJECT_DEPS})
endif()
