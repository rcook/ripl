add_executable(unittest
  $<TARGET_OBJECTS:oplib-test-objs>
  $<TARGET_OBJECTS:ripl-objs>
  $<TARGET_OBJECTS:ripl-test-objs>
  $<TARGET_OBJECTS:riplmain-test-objs>
  $<TARGET_OBJECTS:riplregistry-objs>
  $<TARGET_OBJECTS:riplregistry-test-objs>

  $<TARGET_OBJECTS:plugin-objs>
  $<TARGET_OBJECTS:plugin-test-objs>
  $<TARGET_OBJECTS:utillib-objs>
  $<TARGET_OBJECTS:utillib-test-objs>
  ${SRC}/unittest/main/main.cpp
)
if(NOT NO_USE_CATCH_MAIN)
  add_target_definitions(unittest
    USE_CATCH_MAIN
  )
endif()
target_include_directories(unittest PRIVATE
  ${SRC}/app/test
  ${SRC}/plugin/test
  ${SRC}/riplregistry/test
  ${SRC}/utillib/test
)
target_link_libraries(unittest
  ${CMAKE_DL_LIBS}
  oplib
  riplmain
  testlib
)

set_target_properties(
  unittest
  PROPERTIES FOLDER unittest
)

