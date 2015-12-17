set(RIPLMAINMAIN ${SRC}/riplmain)
set(RIPLMAINTEST ${SRC}/riplmain/test)

add_library(riplmain STATIC
  ${CMAKE_CURRENT_BINARY_DIR}/config.h
  ${RIPLMAINMAIN}/Error.cpp
  ${RIPLMAINMAIN}/Error.h
  ${RIPLMAINMAIN}/Image.h
  ${RIPLMAINMAIN}/Ranges.h
  ${RIPLMAINMAIN}/defs.h
  ${RIPLMAINMAIN}/ripl.h
  ${RIPLMAINMAIN}/riplallc.cpp
  ${RIPLMAINMAIN}/riplallc.h
  ${RIPLMAINMAIN}/ripldbug.h
  ${RIPLMAINMAIN}/ripldefs.h
  ${RIPLMAINMAIN}/riplfilt.cpp
  ${RIPLMAINMAIN}/riplfilt.h
  ${RIPLMAINMAIN}/riplgrey.h
  ${RIPLMAINMAIN}/riplimg.h
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

add_library(riplmain-test-objs OBJECT
  ${CMAKE_CURRENT_BINARY_DIR}/config.h
  ${RIPLMAINTEST}/ImageTest.cpp
  ${RIPLMAINTEST}/data.cpp
  ${RIPLMAINTEST}/data.h
)
target_include_directories(riplmain-test-objs PRIVATE
  ${RIPLMAINMAIN}
  ${SRC}/testlib/public
)

set_target_properties(
  riplmain-test-objs
  riplmain
  PROPERTIES FOLDER riplmain
)

