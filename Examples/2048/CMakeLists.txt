
set(EXAMPLE_NAME 2048)

configure_file(
  qml/run-example.sh.in
  ${CMAKE_CURRENT_BINARY_DIR}/run-example.sh
)

add_custom_target(
  copy-${EXAMPLE_NAME} ALL
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/qml/${EXAMPLE_NAME}.qml ${CMAKE_CURRENT_BINARY_DIR}
)
