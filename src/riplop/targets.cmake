set(RIPLOPMAIN ${SRC}/riplop)

add_library(riplop STATIC
  ${RIPLOPMAIN}/riplentr.cpp
  ${RIPLOPMAIN}/riplop.cpp
  ${RIPLOPMAIN}/riplop.h
)
target_include_directories(riplop PRIVATE
  ${CMAKE_CURRENT_BINARY_DIR}/generated
  ${SRC}/oplib/public
  ${SRC}/riplmain
  ${SRC}/ripltool
)
target_link_libraries(riplop
  oplib
)

set_target_properties(
  riplop
  PROPERTIES FOLDER riplop
)
