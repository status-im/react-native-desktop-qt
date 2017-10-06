
# Copyright (C) 2016, Canonical Ltd.
# All rights reserved.

# This source code is licensed under the BSD-style license found in the
# LICENSE file in the root directory of this source tree. An additional grant
# of patent rights can be found in the PATENTS file in the same directory.

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

find_package(Qt5Core REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Quick REQUIRED)

set(
  SRC
  reactplugin.cpp
  reactvaluecoercion.cpp
  reactblobprovider.cpp
  reactbridge.cpp
  reactdeviceinfo.cpp
  reacteventdispatcher.cpp
  reactsourcecode.cpp
  reactcomponentdata.cpp
  reactmoduledata.cpp
  reactmodulemethod.cpp
  reactpropertyhandler.cpp
  reactexecutor.cpp
  reactnetexecutor.cpp
  reactpipeexecutor.cpp
  reactnetworking.cpp
  reactnetinfo.cpp
  reacttiming.cpp
  reactappstate.cpp
  reactasynclocalstorage.cpp
  reactitem.cpp
  rootview.cpp
  reactviewmanager.cpp
  reactrawtextmanager.cpp
  reacttestmodule.cpp
  reacttextmanager.cpp
  reacttextinputmanager.cpp
  reactimagemanager.cpp
  reactimageloader.cpp
  reactattachedproperties.cpp
  reactuimanager.cpp
  reactredboxitem.cpp
  reactexceptionsmanager.cpp
  reactscrollviewmanager.cpp
  reactslidermanager.cpp
  reactnavigatormanager.cpp
  reactactivityindicatormanager.cpp
  reactbuttonmanager.cpp
  reactmodalmanager.cpp
  layout/flexbox.cpp
  utilities.cpp
  ../../../ReactCommon/yoga/yoga/Yoga.c
  ../../../ReactCommon/yoga/yoga/YGEnums.c
  ../../../ReactCommon/yoga/yoga/YGNodeList.c
)

set(
  QML
  qml/ReactImage.qml
  qml/ReactView.qml
  qml/ReactNavigator.qml
  qml/ReactScrollView.qml
  qml/ReactRedboxItem.qml
  qml/ReactText.qml
  qml/ReactRawText.qml
  qml/ReactActivityIndicator.qml
  qml/ReactButton.qml
  qml/ReactSlider.qml
  qml/ReactModal.qml
)

if(DEFINED REACT_BUILD_STATIC_LIB)
add_library(
  react-native STATIC
  ${SRC}
  ${QML}
  react_resources.qrc
)
else()
add_library(
  react-native SHARED
  ${SRC}
  ${QML}
  react_resources.qrc
)
set_target_properties(
  react-native
  PROPERTIES LIBRARY_OUTPUT_DIRECTORY React
)
endif()

qt5_use_modules(react-native Core Qml Quick)

add_custom_target(
  copy-qmldir ALL
  COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/qmldir ${CMAKE_CURRENT_BINARY_DIR}/React
)
