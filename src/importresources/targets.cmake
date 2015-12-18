set(IMPORTRESOURCESMAIN ${SRC}/importresources/main)
set(RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/resources)

set(IMPORTRESOURCESSRC
  ${IMPORTRESOURCESMAIN}/main.cpp
)

add_executable(importresources
  ${IMPORTRESOURCESSRC}
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/resources.cpp ${CMAKE_CURRENT_BINARY_DIR}/resources.h
  COMMAND importresources ${RESOURCES} ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS ${RESOURCES}/lena.pgm ${IMPORTRESOURCESSRC}
)

set_target_properties(
  importresources
  PROPERTIES FOLDER importresources
)
