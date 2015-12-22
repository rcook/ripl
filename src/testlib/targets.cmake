set(TESTLIBMAIN ${SRC}/testlib/main)
set(TESTLIBPUBLIC ${SRC}/testlib/public)

add_library(testlib STATIC
  ${SHAREDSRCS}
  ${TESTLIBMAIN}/helper.cpp
  ${TESTLIBMAIN}/logging.cpp
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
