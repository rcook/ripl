set(TESTMAIN ${SRC}/test)

add_executable(ripltest
  ${TESTMAIN}/AddTest.cpp
  ${TESTMAIN}/ImageTest.cpp
  ${TESTMAIN}/data.cpp
  ${TESTMAIN}/data.h
  ${TESTMAIN}/helper.cpp
  ${TESTMAIN}/helper.h
  ${TESTMAIN}/main.cpp
)
if(NOT NO_USE_CATCH_MAIN)
  add_target_definitions(ripltest USE_CATCH_MAIN)
endif()
target_include_directories(ripltest PRIVATE
  ${SRC}/oplib
  ${SRC}/riplmain
)
target_link_libraries(ripltest
  oplib
  riplmain
  riplop
  ripltool
)

