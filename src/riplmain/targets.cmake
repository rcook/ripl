set(RIPLMAINMAIN ${SRC}/riplmain)
set(RIPLMAINTEST ${SRC}/riplmain/test)

add_library(riplmain SHARED
  ${SHAREDSRCS}
  ${RIPLMAINMAIN}/Error.cpp
  ${RIPLMAINMAIN}/Error.h
  ${RIPLMAINMAIN}/Image.h
  ${RIPLMAINMAIN}/Op.cpp
  ${RIPLMAINMAIN}/Op.h
  ${RIPLMAINMAIN}/Ranges.h
  ${RIPLMAINMAIN}/RegistryImpl.cpp
  ${RIPLMAINMAIN}/RegistryImpl.h
  ${RIPLMAINMAIN}/ripl.h
  ${RIPLMAINMAIN}/ripldbug.h
  ${RIPLMAINMAIN}/ripldefs.h
  ${RIPLMAINMAIN}/riplfilt.cpp
  ${RIPLMAINMAIN}/riplfilt.h
  ${RIPLMAINMAIN}/riplgrey.h
  ${RIPLMAINMAIN}/riplimg.h
  ${RIPLMAINMAIN}/riplmain-defs.h
  ${RIPLMAINMAIN}/riplmisc.cpp
  ${RIPLMAINMAIN}/riplmisc.h
  ${RIPLMAINMAIN}/riplmsg.cpp
  ${RIPLMAINMAIN}/riplmsg.h
  ${RIPLMAINMAIN}/riplpars.cpp
  ${RIPLMAINMAIN}/riplpars.h
  ${RIPLMAINMAIN}/riplpbm.cpp
  ${RIPLMAINMAIN}/riplpbm.h
  ${RIPLMAINMAIN}/riplrgb.cpp
  ${RIPLMAINMAIN}/riplrgb.h
  ${RIPLMAINMAIN}/validate.h
)

target_compile_definitions(riplmain PRIVATE BUILD_RIPLMAIN)

# $TODO: Eliminate circular dependency between riplmain and ripllib by merging them!
target_include_directories(riplmain PRIVATE
  ${RIPLMAINMAIN}
  ${SRC}/ripllib/public
  ${SRC}/utillib/public
)

target_link_libraries(riplmain
  utillib
)

add_library(riplmain-test-objs OBJECT
  ${CMAKE_CURRENT_BINARY_DIR}/generated/resources.h
  ${RIPLMAINTEST}/ImageTest.cpp
  ${RIPLMAINTEST}/NetpbmTest.cpp
  ${RIPLMAINTEST}/OpTest.cpp
  ${RIPLMAINTEST}/data.cpp
  ${RIPLMAINTEST}/data.h
)

target_include_directories(riplmain-test-objs PRIVATE
  ${RIPLMAINMAIN}
  ${SRC}/ripllib/public
  ${SRC}/testlib/public
)

set_target_properties(
  riplmain-test-objs
  riplmain
  PROPERTIES FOLDER riplmain
)
