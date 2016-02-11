cmake_minimum_required(VERSION 3.2)

set(APP_NAME <%= name %>)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../node_modules/react-native/React/Layout)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../node_modules/react-native/ReactUbuntu ${CMAKE_CURRENT_BINARY_DIR}/ReactUbuntu)

configure_file(
  qml/run-app.sh.in
  ${CMAKE_CURRENT_BINARY_DIR}/run-app.sh
)

