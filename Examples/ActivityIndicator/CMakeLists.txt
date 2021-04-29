
set(EXAMPLE_NAME ActivityIndicator)

configure_file(
  ../run-example.sh.in
  ${CMAKE_CURRENT_BINARY_DIR}/run-example.sh
)

configure_file(
  ../Example.qml.in
  ${CMAKE_CURRENT_BINARY_DIR}/${EXAMPLE_NAME}.qml
)
