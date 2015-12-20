set(RIPLMAIN ${SRC}/ripl/main)

add_executable(ripl
  ${RIPLMAIN}/main.cpp
)
target_include_directories(ripl PRIVATE
  ${CMAKE_CURRENT_BINARY_DIR}/generated
  ${SRC}/riplmain
)
target_link_libraries(ripl
  oplib
  riplmain
  riplop
  ripltool
)

set_target_properties(
  ripl
  PROPERTIES FOLDER ripl
)

