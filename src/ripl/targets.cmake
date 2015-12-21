set(RIPLMAIN ${SRC}/ripl/main)
set(RIPLTEST ${SRC}/ripl/test)

add_library(ripl-objs OBJECT
  ${RIPLMAIN}/PluginManager.cpp
  ${RIPLMAIN}/PluginManager.h
  ${RIPLMAIN}/riplentr.cpp
  ${RIPLMAIN}/riplentr.h
  ${RIPLMAIN}/riplop.cpp
  ${RIPLMAIN}/riplop.h
)
target_include_directories(ripl-objs PRIVATE
  ${CMAKE_CURRENT_BINARY_DIR}/generated
  ${SRC}/riplmain
  ${SRC}/riplregistry/public
  ${SRC}/shared
  ${SRC}/utillib/public
)

add_executable(ripl
  $<TARGET_OBJECTS:ripl-objs>
  ${RIPLMAIN}/main.cpp
)
target_include_directories(ripl PRIVATE
  ${CMAKE_CURRENT_BINARY_DIR}/generated
  ${RIPLMAIN}
  ${SRC}/riplmain
  ${SRC}/riplregistry/public
  ${SRC}/shared
  ${SRC}/utillib/public
)
target_link_libraries(ripl
  ${CMAKE_DL_LIBS}
  riplmain
  riplregistry
  utillib
)

set_target_properties(
  ripl-objs
  ripl
  PROPERTIES FOLDER ripl
)

