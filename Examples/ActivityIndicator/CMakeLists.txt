
set(EXAMPLE_NAME ActivityIndicator)

configure_file(
  qml/run-example.sh.in
  ${CMAKE_CURRENT_BINARY_DIR}/run-example.sh
)

add_custom_target(
  copy-${EXAMPLE_NAME} ALL
  COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/qml/${EXAMPLE_NAME}.qml ${CMAKE_CURRENT_BINARY_DIR}
)
