set(UNITTESTMAIN ${SRC}/unittest/main)

add_executable(unittest
  ${CMAKE_CURRENT_BINARY_DIR}/generated/resources.cpp
  ${SHAREDSRCS}
  $<TARGET_OBJECTS:oplib-objs>
  $<TARGET_OBJECTS:oplib-test-objs>
  $<TARGET_OBJECTS:ripl-objs>
  $<TARGET_OBJECTS:ripl-test-objs>
  $<TARGET_OBJECTS:ripllib-test-objs>
  $<TARGET_OBJECTS:riplmain-test-objs>
  $<TARGET_OBJECTS:utillib-test-objs>
  ${UNITTESTMAIN}/main.cpp
)

if(NOT NO_USE_CATCH_MAIN)
  target_compile_definitions(unittest PRIVATE USE_CATCH_MAIN)
endif()

target_include_directories(unittest PRIVATE
  ${SRC}/ripllib/test
  ${SRC}/riplmain
  ${SRC}/testlib/public
  ${SRC}/utillib/public
)

target_link_libraries(unittest
  ${CMAKE_DL_LIBS}
  ripllib
  riplmain
  ripltool
  testlib
  utillib
)

set_target_properties(
  unittest
  PROPERTIES FOLDER unittest
)
