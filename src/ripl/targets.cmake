set(RIPLMAIN ${SRC}/ripl/main)

add_executable(ripl
  ${RIPLMAIN}/main.cpp
  ${RIPLMAIN}/riplentr.cpp
  ${RIPLMAIN}/riplop.cpp
  ${RIPLMAIN}/riplop.h
)
target_include_directories(ripl PRIVATE
  ${CMAKE_CURRENT_BINARY_DIR}/generated
  ${SRC}/oplib/public
  ${SRC}/riplmain
)
target_link_libraries(ripl
  oplib
  riplmain
  ripltool
)

set_target_properties(
  ripl
  PROPERTIES FOLDER ripl
)

