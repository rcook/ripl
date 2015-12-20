add_library(app-objs OBJECT
  ${CMAKE_CURRENT_BINARY_DIR}/generated/config.h
  ${SRC}/app/main/Op.cpp
  ${SRC}/app/main/Op.h
  ${SRC}/app/main/PluginManager.cpp
  ${SRC}/app/main/PluginManager.h
  ${SRC}/app/main/RegistryImpl.cpp
  ${SRC}/app/main/RegistryImpl.h
)
target_compile_definitions(app-objs PRIVATE
  BUILD_APP
)
target_include_directories(app-objs PRIVATE
  ${CMAKE_CURRENT_BINARY_DIR}/generated
  ${SRC}/riplregistry/public
  ${SRC}/shared
  ${SRC}/utillib/public
)

add_library(app-test-objs OBJECT
  ${CMAKE_CURRENT_BINARY_DIR}/generated/config.h
  ${SRC}/app/test/OpTest.cpp
)
target_include_directories(app-test-objs PRIVATE
  ${CMAKE_CURRENT_BINARY_DIR}/generated
  ${SRC}/app/main
  ${SRC}/riplregistry/public
)

add_executable(app
  $<TARGET_OBJECTS:app-objs>
  ${SRC}/app/main/main.cpp
)
target_include_directories(app PRIVATE
  ${CMAKE_CURRENT_BINARY_DIR}/generated
  ${SRC}/riplregistry/public
  ${SRC}/utillib/public
)
target_link_libraries(app
  ${CMAKE_DL_LIBS}
  riplregistry
  utillib
)

set_target_properties(
  app-objs
  app-test-objs
  app
  PROPERTIES FOLDER app
)

