cmake_minimum_required(VERSION 3.2)

set(APP_NAME <%= name %>)

configure_file(
  qml/run-app.sh.in
  ${CMAKE_CURRENT_BINARY_DIR}/run-app.sh
)

add_custom_target(
  copy-${APP_NAME} ALL
  COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/qml/app.qml ${CMAKE_CURRENT_BINARY_DIR}
)
