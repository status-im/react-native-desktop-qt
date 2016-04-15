set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

find_package(Qt5Core REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Quick REQUIRED)


configure_file(
  main.qml.in
  ${CMAKE_CURRENT_SOURCE_DIR}/main.qml
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../runtime/src)

add_executable(
  ${APP_NAME}
  main.cpp
  main.qrc
)

qt5_use_modules(${APP_NAME} Core Qml Quick)

target_link_libraries(
  ${APP_NAME}
  react-native
)


