set(RIPLOPMAIN ${SRC}/riplop)

add_library(riplop STATIC
  ${RIPLOPMAIN}/riplentr.cpp
  ${RIPLOPMAIN}/riplop.cpp
  ${RIPLOPMAIN}/riplop.h
  ${RIPLOPMAIN}/riplreg.cpp
)
target_include_directories(riplop PRIVATE
  ${SRC}/oplib
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
