set(RIPLMAIN ${SRC}/ripl/main)
set(RIPLTEST ${SRC}/ripl/test)

add_library(ripl-objs OBJECT
  ${SHAREDSRCS}
  ${RIPLMAIN}/Op.cpp
  ${RIPLMAIN}/Op.h
  ${RIPLMAIN}/PluginManager.cpp
  ${RIPLMAIN}/PluginManager.h
  ${RIPLMAIN}/plugin_helper.cpp
  ${RIPLMAIN}/plugin_helper.h
  ${RIPLMAIN}/RegistryImpl.cpp
  ${RIPLMAIN}/RegistryImpl.h
  ${RIPLMAIN}/riplentr.cpp
  ${RIPLMAIN}/riplentr.h
)

target_include_directories(ripl-objs PRIVATE
  ${SRC}/ripllib/public
  ${SRC}/riplmain
  ${SRC}/utillib/public
)

add_executable(ripl
  ${SHAREDSRCS}
  $<TARGET_OBJECTS:ripl-objs>
  ${RIPLMAIN}/main.cpp
)

target_include_directories(ripl PRIVATE
  ${RIPLMAIN}
  ${SRC}/ripllib/public
  ${SRC}/riplmain
  ${SRC}/utillib/public
)

target_link_libraries(ripl
  ${CMAKE_DL_LIBS}
  ripllib
  riplmain
  utillib
)

add_library(ripl-test-objs OBJECT
  ${RIPLTEST}/OpTest.cpp
)

target_include_directories(ripl-test-objs PRIVATE
  ${RIPLMAIN}
  ${SRC}/ripllib/public
  ${SRC}/riplmain
)

set_target_properties(
  ripl-objs
  ripl
  ripl-test-objs
  PROPERTIES FOLDER ripl
)
