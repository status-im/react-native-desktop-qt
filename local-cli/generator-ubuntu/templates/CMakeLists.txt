cmake_minimum_required(VERSION 2.8.11)

set(APP_NAME <%= name %>)
set(REACT_BUILD_STATIC_LIB ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../node_modules/react-native/React/Layout)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../node_modules/react-native/ReactUbuntu/runtime/src ${CMAKE_CURRENT_BINARY_DIR}/lib)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../node_modules/react-native/ReactUbuntu/application/src ${CMAKE_CURRENT_BINARY_DIR}/bin)

configure_file(
  run-app.sh.in
  ${CMAKE_CURRENT_BINARY_DIR}/run-app.sh
)

