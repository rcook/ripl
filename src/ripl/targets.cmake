set(RIPLMAIN ${SRC}/ripl)

add_executable(ripl
  ${RIPLMAIN}/riplcmd.cpp
)
target_include_directories(ripl PRIVATE
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
