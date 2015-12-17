set(RIPLCMDMAIN ${SRC}/riplcmd)

add_executable(ripl
  ${RIPLCMDMAIN}/riplcmd.cpp
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

