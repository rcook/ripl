set(RIPLMAIN ${SRC}/ripl/main)
set(RIPLTEST ${SRC}/ripl/test)

add_library(ripl-objs OBJECT
  ${SHAREDSRCS}
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
  ${SRC}/riplmain
  ${SRC}/riplregistry/public
  ${SRC}/utillib/public
)

add_executable(ripl
  ${SHAREDSRCS}
  $<TARGET_OBJECTS:ripl-objs>
  ${RIPLMAIN}/main.cpp
)

target_include_directories(ripl PRIVATE
  ${RIPLMAIN}
  ${SRC}/riplmain
  ${SRC}/riplregistry/public
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
