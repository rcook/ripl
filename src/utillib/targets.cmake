set(UTILLIBMAIN ${SRC}/utillib/main)
set(UTILLIBPUBLIC ${SRC}/utillib/public)

add_library(utillib STATIC
  ${SHAREDSRCS}
  ${UTILLIBMAIN}/MemoryBuffer.cpp
  ${UTILLIBMAIN}/OSError.cpp
  ${UTILLIBMAIN}/fs.cpp
  ${UTILLIBMAIN}/string.cpp
  ${UTILLIBMAIN}/validate.h
  ${UTILLIBPUBLIC}/utillib/MemoryBuffer.h
  ${UTILLIBPUBLIC}/utillib/OSError.h
  ${UTILLIBPUBLIC}/utillib/ScopedHandle.h
  ${UTILLIBPUBLIC}/utillib/Trie.h
  ${UTILLIBPUBLIC}/utillib/fs.h
  ${UTILLIBPUBLIC}/utillib/string.h
  ${UTILLIBPUBLIC}/utillib/validate.h
)

target_include_directories(utillib PRIVATE
  ${UTILLIBPUBLIC}
)

add_library(utillib-test-objs OBJECT
  ${SHAREDSRCS}
  ${SRC}/utillib/test/ScopedHandleTest.cpp
  ${SRC}/utillib/test/StringTest.cpp
  ${SRC}/utillib/test/TrieTest.cpp
)

target_include_directories(utillib-test-objs PRIVATE
  ${UTILLIBPUBLIC}
)

set_target_properties(
  utillib
  utillib-test-objs
  PROPERTIES FOLDER utillib
)
