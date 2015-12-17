set(RIPLTOOLMAIN ${SRC}/ripltool)

add_library(ripltool STATIC
  ${RIPLTOOLMAIN}/riplfft.cpp
  ${RIPLTOOLMAIN}/riplfft.h
  ${RIPLTOOLMAIN}/riplrand.cpp
  ${RIPLTOOLMAIN}/riplrand.h
  ${RIPLTOOLMAIN}/ripltdfs.h
  ${RIPLTOOLMAIN}/riplwave.cpp
  ${RIPLTOOLMAIN}/riplwave.h
)
target_include_directories(ripltool PRIVATE
  ${SRC}/riplmain
)

