set(RIPLMAIN ${SRC}/ripl/main)
set(RIPLTEST ${SRC}/ripl/test)

add_library(ripl-objs OBJECT
  ${RIPLMAIN}/Op.cpp
  ${RIPLMAIN}/Op.h
  ${RIPLMAIN}/PluginManager.cpp
  ${RIPLMAIN}/PluginManager.h
  ${RIPLMAIN}/RegistryImpl.cpp
  ${RIPLMAIN}/RegistryImpl.h
  ${RIPLMAIN}/riplentr.cpp
  ${RIPLMAIN}/riplentr.h
  ${RIPLMAIN}/riplop.cpp
  ${RIPLMAIN}/riplop.h
)
target_include_directories(ripl-objs PRIVATE
  ${CMAKE_CURRENT_BINARY_DIR}/generated
  ${SRC}/oplib/public
  ${SRC}/riplmain
  ${SRC}/riplregistry/public
  ${SRC}/shared
  ${SRC}/utillib/public
)

add_library(ripl-test-objs OBJECT
  ${RIPLTEST}/OpTest.cpp
)
target_include_directories(ripl-test-objs PRIVATE
  ${CMAKE_CURRENT_BINARY_DIR}/generated
  ${RIPLMAIN}
  ${SRC}/riplregistry/public
  ${SRC}/shared
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
  oplib
  riplmain
  riplregistry
  utillib
)

set_target_properties(
  ripl-test-objs
  ripl-objs
  ripl
  PROPERTIES FOLDER ripl
)

