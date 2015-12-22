set(UNITTESTMAIN ${SRC}/unittest/main)

add_executable(unittest
  ${CMAKE_CURRENT_BINARY_DIR}/generated/resources.cpp
  ${SHAREDSRCS}
  $<TARGET_OBJECTS:oplib-objs>
  $<TARGET_OBJECTS:oplib-test-objs>
  $<TARGET_OBJECTS:riplmain-test-objs>
  $<TARGET_OBJECTS:riplregistry-test-objs>
  $<TARGET_OBJECTS:utillib-test-objs>
  ${UNITTESTMAIN}/main.cpp
)

if(NOT NO_USE_CATCH_MAIN)
  target_compile_definitions(unittest PRIVATE USE_CATCH_MAIN)
endif()

target_include_directories(unittest PRIVATE
  ${SRC}/riplmain
  ${SRC}/riplregistry/test
  ${SRC}/testlib/public
  ${SRC}/utillib/public
)

target_link_libraries(unittest
  ${CMAKE_DL_LIBS}
  riplmain
  riplregistry
  ripltool
  testlib
  utillib
)

set_target_properties(
  unittest
  PROPERTIES FOLDER unittest
)

