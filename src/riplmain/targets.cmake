set(RIPLMAINMAIN ${SRC}/riplmain)
set(RIPLMAINTEST ${SRC}/riplmain/test)

add_library(riplmain STATIC
  ${SHAREDSRCS}
  ${RIPLMAINMAIN}/Error.cpp
  ${RIPLMAINMAIN}/Error.h
  ${RIPLMAINMAIN}/Image.h
  ${RIPLMAINMAIN}/Op.cpp
  ${RIPLMAINMAIN}/Op.h
  ${RIPLMAINMAIN}/Ranges.h
  ${RIPLMAINMAIN}/RegistryImpl.cpp
  ${RIPLMAINMAIN}/RegistryImpl.h
  ${RIPLMAINMAIN}/defs.h
  ${RIPLMAINMAIN}/ripl.h
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

# $TODO: Eliminate circular dependency between riplmain and riplregistry!
target_include_directories(riplmain PRIVATE
  ${RIPLMAINMAIN}
  ${SRC}/riplregistry/public
  ${SRC}/utillib/public
)

add_library(riplmain-test-objs OBJECT
  ${CMAKE_CURRENT_BINARY_DIR}/generated/resources.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/generated/resources.h
  ${RIPLMAINTEST}/ImageTest.cpp
  ${RIPLMAINTEST}/NetpbmTest.cpp
  ${RIPLMAINTEST}/OpTest.cpp
  ${RIPLMAINTEST}/data.cpp
  ${RIPLMAINTEST}/data.h
)

target_include_directories(riplmain-test-objs PRIVATE
  ${RIPLMAINMAIN}
  ${SRC}/riplregistry/public
  ${SRC}/testlib/public
)

set_target_properties(
  riplmain-test-objs
  riplmain
  PROPERTIES FOLDER riplmain
)
