
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

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  ADD_DEFINITIONS(-DRCT_DEV)
endif()

find_package(Qt5Core REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(Qt5WebSockets REQUIRED)
find_package(Qt5Svg REQUIRED)
find_package(Qt5WebEngine REQUIRED)

if (USE_QTWEBKIT)
  find_package(Qt5WebKit REQUIRED)
endif()

if(JAVASCRIPTCORE_ENABLED)
  ADD_DEFINITIONS(-DJAVASCRIPTCORE_ENABLED)

  include_directories("../../../ReactCommon")
  include_directories("../../../ReactAndroid/src/main/jni/third-party")

  include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)

  set(JavaScriptCore_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/JavaScriptCore_ep-prefix/src/JavaScriptCore_ep")
  set(JavaScriptCore_INCLUDE_DIR "${JavaScriptCore_PREFIX}/WebKitBuild/Release/DerivedSources/ForwardingHeaders")
  set(JavaScriptCore_STATIC_LIB
      "${JavaScriptCore_PREFIX}/WebKitBuild/Release/lib/${CMAKE_STATIC_LIBRARY_PREFIX}JavaScriptCore${CMAKE_STATIC_LIBRARY_SUFFIX}")
  set(bmalloc_STATIC_LIB
       "${JavaScriptCore_PREFIX}/WebKitBuild/Release/lib/${CMAKE_STATIC_LIBRARY_PREFIX}bmalloc${CMAKE_STATIC_LIBRARY_SUFFIX}")
  set(WTF_STATIC_LIB
       "${JavaScriptCore_PREFIX}/WebKitBuild/Release/lib/${CMAKE_STATIC_LIBRARY_PREFIX}WTF${CMAKE_STATIC_LIBRARY_SUFFIX}")

  include_directories(${JavaScriptCore_INCLUDE_DIR})

  ExternalProject_Add(JavaScriptCore_ep
    GIT_REPOSITORY git@github.com:WebKit/webkit.git
    GIT_TAG 06fcd5bda23a9f4844263b60b33f2ad83be4d871
    BUILD_BYPRODUCTS ${JavaScriptCore_STATIC_LIB}
    CONFIGURE_COMMAND ${JavaScriptCore_PREFIX}/Tools/Scripts/build-webkit --jsc-only --cmakeargs="-DENABLE_STATIC_JSC=ON -DUSE_THIN_ARCHIVES=OFF"
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
  )

  set(FOLLY_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/folly_ep-prefix/src/folly_ep-install")
  set(FOLLY_INCLUDE_DIR "${FOLLY_PREFIX}/include")
  set(FOLLY_STATIC_LIB "${FOLLY_PREFIX}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}folly${CMAKE_STATIC_LIBRARY_SUFFIX}")
  set(FOLLY_CMAKE_ARGS
      -DCMAKE_INSTALL_PREFIX:PATH=${FOLLY_PREFIX}
      -DCMAKE_POSITION_INDEPENDENT_CODE=ON)

  include_directories(${FOLLY_INCLUDE_DIR})
  MESSAGE(STATUS "FOLLY_INCLUDE_DIR = ${FOLLY_INCLUDE_DIR}")

  ExternalProject_Add(folly_ep
        URL "https://github.com/facebook/folly/archive/v2018.04.30.00.tar.gz"
        BUILD_BYPRODUCTS "${FOLLY_STATIC_LIB}"
        CMAKE_ARGS ${FOLLY_CMAKE_ARGS})

  set(
    SRC_JAVASCRIPTCORE
    ../../../ReactCommon/jschelpers/JSCHelpers.cpp
    ../../../ReactCommon/jschelpers/Unicode.cpp
    ../../../ReactCommon/jschelpers/Value.cpp
    ../../../ReactCommon/jschelpers/JSCWrapper.cpp
    ../../../ReactCommon/jschelpers/systemJSCWrapper.cpp

    ../../../ReactCommon/jsinspector/InspectorInterfaces.cpp

    ../../../ReactCommon/privatedata/PrivateDataBase.cpp

    ../../../ReactCommon/cxxreact/CxxNativeModule.cpp
    ../../../ReactCommon/cxxreact/Instance.cpp
    ../../../ReactCommon/cxxreact/JSBigString.cpp
    ../../../ReactCommon/cxxreact/JSBundleType.cpp
    ../../../ReactCommon/cxxreact/JSCExecutor.cpp
    ../../../ReactCommon/cxxreact/JSCLegacyTracing.cpp
    ../../../ReactCommon/cxxreact/JSCMemory.cpp
    ../../../ReactCommon/cxxreact/JSCNativeModules.cpp
    ../../../ReactCommon/cxxreact/JSCPerfStats.cpp
    ../../../ReactCommon/cxxreact/JSCSamplingProfiler.cpp
    ../../../ReactCommon/cxxreact/JSCTracing.cpp
    ../../../ReactCommon/cxxreact/JSCUtils.cpp
    ../../../ReactCommon/cxxreact/JSIndexedRAMBundle.cpp
    ../../../ReactCommon/cxxreact/MethodCall.cpp
    ../../../ReactCommon/cxxreact/ModuleRegistry.cpp
    ../../../ReactCommon/cxxreact/NativeToJsBridge.cpp
    ../../../ReactCommon/cxxreact/Platform.cpp
    ../../../ReactCommon/cxxreact/RAMBundleRegistry.cpp
    jscutilities.cpp
    communication/javascriptcoreexecutor.cpp)
endif()

set(
  SRC_RUNTIME
  ${SRC_JAVASCRIPTCORE}
  reactplugin.cpp
  valuecoercion.cpp
  blobprovider.cpp
  bridge.cpp
  deviceinfo.cpp
  platform.cpp
  eventdispatcher.cpp
  sourcecode.cpp
  componentdata.cpp
  moduledata.cpp
  modulemethod.cpp
  propertyhandler.cpp
  networking.cpp
  netinfo.cpp
  timing.cpp
  appstate.cpp
  asynclocalstorage.cpp
  reactitem.cpp
  rootview.cpp
  reactnetworkaccessmanager.cpp
  mouseeventsinterceptor.cpp
  testmodule.cpp
  attachedproperties.cpp
  uimanager.cpp
  redbox.cpp
  exceptionsmanager.cpp
  clipboard.cpp
  linkingmanager.cpp
  alert.cpp
  componentmanagers/slidermanager.cpp
  componentmanagers/viewmanager.cpp
  componentmanagers/rawtextmanager.cpp
  componentmanagers/scrollviewmanager.cpp
  componentmanagers/navigatormanager.cpp
  componentmanagers/activityindicatormanager.cpp
  componentmanagers/buttonmanager.cpp
  componentmanagers/modalmanager.cpp
  componentmanagers/textmanager.cpp
  componentmanagers/textinputmanager.cpp
  componentmanagers/imagemanager.cpp
  componentmanagers/imageloader.cpp
  componentmanagers/pickermanager.cpp
  componentmanagers/switchmanager.cpp
  componentmanagers/webviewmanager.cpp
  componentmanagers/scrollviewmodel.cpp
  layout/flexbox.cpp
  utilities.cpp
  communication/serverconnection.cpp
  communication/nodejsexecutor.cpp
  communication/websocketexecutor.cpp
  communication/jswebengineexecutor.cpp
  communication/ijsexecutor.h
  websocketmodule.cpp
  ../../../ReactCommon/yoga/yoga/Yoga.cpp
  ../../../ReactCommon/yoga/yoga/YGEnums.cpp
  ../../../ReactCommon/yoga/yoga/YGNode.cpp
  ../../../ReactCommon/yoga/yoga/YGNodePrint.cpp
  ../../../ReactCommon/yoga/yoga/YGStyle.cpp
  ../../../ReactCommon/yoga/yoga/YGLayout.cpp
  ../../../ReactCommon/yoga/yoga/YGEnums.cpp
  ../../../ReactCommon/yoga/yoga/Utils.cpp
  ../../../ReactCommon/yoga/yoga/YGFloatOptional.cpp
  ../../../ReactCommon/yoga/yoga/YGConfig.cpp
)

message(STATUS "REACT_NATIVE_DESKTOP_EXTERNAL_MODULES_SRC = ${REACT_NATIVE_DESKTOP_EXTERNAL_MODULES_SRC}")

set (SRC ${SRC_RUNTIME} ${REACT_NATIVE_DESKTOP_EXTERNAL_MODULES_SRC})

set(
  QML
  qml/ReactImage.qml
  qml/ReactView.qml
  qml/ReactNavigator.qml
  qml/ReactScrollView.qml
  qml/ReactScrollListView.qml
  qml/ReactRedboxItem.qml
  qml/ReactText.qml
  qml/ReactRawText.qml
  qml/ReactActivityIndicator.qml
  qml/ReactButton.qml
  qml/ReactSlider.qml
  qml/ReactModal.qml
  qml/DevMenu.qml
  qml/ReactTextInput.qml
  qml/ReactTextInputArea.qml
  qml/ReactTextInputField.qml
  qml/ReactAlert.qml
)

if (USE_QTWEBKIT)
  set(QML ${QML} qml/ReactQtWebKitWebView.qml)
  ADD_DEFINITIONS(-DUSE_QTWEBKIT)
else()
  set(QML ${QML} qml/ReactWebView.qml)
endif()

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

if (REACT_NATIVE_DESKTOP_EXTERNAL_QT_COMPONENTS)
  find_package(Qt5 COMPONENTS ${REACT_NATIVE_DESKTOP_EXTERNAL_QT_COMPONENTS} REQUIRED)
endif()

if (REACT_NATIVE_DESKTOP_EXTERNAL_PROJECT_DEPS)
  add_dependencies(react-native ${REACT_NATIVE_DESKTOP_EXTERNAL_PROJECT_DEPS})
endif()

if (REACT_NATIVE_DESKTOP_EXTERNAL_MODULES_LIBS)
  target_link_libraries(react-native ${REACT_NATIVE_DESKTOP_EXTERNAL_MODULES_LIBS})
endif()

if(JAVASCRIPTCORE_ENABLED)
  add_dependencies(react-native JavaScriptCore_ep folly_ep)

  target_link_libraries(react-native
    ${JavaScriptCore_STATIC_LIB} ${bmalloc_STATIC_LIB} ${WTF_STATIC_LIB}
    "-licudata"
    "-licui18n"
    "-licuuc"
    "-lglog"
    "-ldouble-conversion"
    ${FOLLY_STATIC_LIB}
  )
endif()

include_directories(${REACT_NATIVE_DESKTOP_EXTERNAL_MODULES_INCLUDE_DIRS})

set(USED_QT_MODULES Core Qml Quick WebSockets Svg Concurrent WebEngineWidgets)
if (USE_QTWEBKIT)
  set (USED_QT_MODULES ${USED_QT_MODULES} WebKit)
endif()

qt5_use_modules(react-native ${USED_QT_MODULES})

add_custom_target(
  copy-qmldir ALL
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/qmldir ${CMAKE_CURRENT_BINARY_DIR}/React
)
