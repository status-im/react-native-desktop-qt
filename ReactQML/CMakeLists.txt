
set(CMAKE_INcLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(Qt5Core REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Quick REQUIRED)

add_library(
  react-native MODULE

  reactplugin.cpp
  reactvaluecoercion.cpp
  reactbridge.cpp
  reactcomponentdata.cpp
  reactmoduledata.cpp
  reactmodulemethod.cpp
  reactexecutor.cpp
  reactnetexecutor.cpp
  reactpipeexecutor.cpp
  reactnetworking.cpp
  reacttiming.cpp
  reactview.cpp
  ubuntuactivitymanager.cpp
  ubuntudatepickermanager.cpp
  ubunturawtextmanager.cpp
  ubuntuscrollviewmanager.cpp
  ubuntutextmanager.cpp
  ubuntuuimanager.cpp
  ubuntuview.cpp
  ubuntuviewmanager.cpp
)

set_target_properties(
  react-native
  PROPERTIES LIBRARY_OUTPUT_DIRECTORY React
 )

qt5_use_modules(react-native Core Qml Quick)

add_custom_target(
  copy-qmldir ALL
  COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/qmldir ${CMAKE_CURRENT_BINARY_DIR}/React
 )
