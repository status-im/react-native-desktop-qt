cmake_minimum_required(VERSION 2.8.11)

project(react-native-ubuntu)

set(PROJECT_NAME "react-native-ubuntu")
set(PROJECT_VERSION "0.0.1")

include_directories(./React/Layout)

add_subdirectory(ReactUbuntu)
add_subdirectory(Examples)

add_custom_target(
  "watch"
  COMMAND node -e \"require('fs').watch('${CMAKE_SOURCE_DIR}/ReactUbuntu',function(e,f){require('child_process').spawn('make',{stdio:'inherit'})})\"
  DEPENDS react-native
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
