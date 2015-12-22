set(TESTLIBMAIN ${SRC}/testlib/main)
set(TESTLIBPUBLIC ${SRC}/testlib/public)

add_library(testlib STATIC
  ${SHAREDSRCS}
  ${TESTLIBMAIN}/MemoryBuffer.cpp
  ${TESTLIBMAIN}/helper.cpp
  ${TESTLIBMAIN}/logging.cpp
  ${TESTLIBPUBLIC}/testlib/MemoryBuffer.h
  ${TESTLIBPUBLIC}/testlib/helper.h
  ${TESTLIBPUBLIC}/testlib/logging.h
)

target_include_directories(testlib PRIVATE
  ${SRC}/riplmain
  ${TESTLIBPUBLIC}
)

set_target_properties(
  testlib
  PROPERTIES FOLDER testlib
)
