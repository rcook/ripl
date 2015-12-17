set(TESTLIBMAIN ${SRC}/testlib/main)
set(TESTLIBPUBLIC ${SRC}/testlib/public)

add_library(testlib-objs OBJECT
  ${CMAKE_CURRENT_BINARY_DIR}/config.h
  ${TESTLIBMAIN}/helper.cpp
  ${TESTLIBPUBLIC}/testlib/helper.h
)
target_include_directories(testlib-objs PRIVATE
  ${SRC}/riplmain
  ${TESTLIBPUBLIC}
)

add_library(testlib STATIC
  $<TARGET_OBJECTS:testlib-objs>
)

set_target_properties(
  testlib-objs
  PROPERTIES FOLDER testlib
)

