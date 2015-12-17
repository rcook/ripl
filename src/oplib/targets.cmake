set(OPLIBMAIN ${SRC}/oplib)
set(OPLIBTEST ${SRC}/oplib/test)

add_library(oplib STATIC
  ${OPLIBMAIN}/add.cpp
  ${OPLIBMAIN}/add.h
  ${OPLIBMAIN}/ahe.cpp
  ${OPLIBMAIN}/ahe.h
  ${OPLIBMAIN}/binmorph.cpp
  ${OPLIBMAIN}/binmorph.h
  ${OPLIBMAIN}/carve.cpp
  ${OPLIBMAIN}/carve.h
  ${OPLIBMAIN}/conv.cpp
  ${OPLIBMAIN}/conv.h
  ${OPLIBMAIN}/dllconv.cpp
  ${OPLIBMAIN}/dllconv.h
  ${OPLIBMAIN}/fftfilt.cpp
  ${OPLIBMAIN}/fftfilt.h
  ${OPLIBMAIN}/gauss.cpp
  ${OPLIBMAIN}/gauss.h
  ${OPLIBMAIN}/glmorph.cpp
  ${OPLIBMAIN}/glmorph.h
  ${OPLIBMAIN}/globalhe.cpp
  ${OPLIBMAIN}/globalhe.h
  ${OPLIBMAIN}/magsynth.cpp
  ${OPLIBMAIN}/magsynth.h
  ${OPLIBMAIN}/marrhild.cpp
  ${OPLIBMAIN}/marrhild.h
  ${OPLIBMAIN}/mask.cpp
  ${OPLIBMAIN}/mask.h
  ${OPLIBMAIN}/median.cpp
  ${OPLIBMAIN}/median.h
  ${OPLIBMAIN}/misc.cpp
  ${OPLIBMAIN}/misc.h
  ${OPLIBMAIN}/nop.cpp
  ${OPLIBMAIN}/nop.h
  ${OPLIBMAIN}/phsynth.cpp
  ${OPLIBMAIN}/phsynth.h
  ${OPLIBMAIN}/plane.cpp
  ${OPLIBMAIN}/plane.h
  ${OPLIBMAIN}/siahe.cpp
  ${OPLIBMAIN}/siahe.h
  ${OPLIBMAIN}/siahe2.cpp
  ${OPLIBMAIN}/siahe2.h
  ${OPLIBMAIN}/sobel.cpp
  ${OPLIBMAIN}/sobel.h
  ${OPLIBMAIN}/spectrum.cpp
  ${OPLIBMAIN}/spectrum.h
  ${OPLIBMAIN}/test.cpp
  ${OPLIBMAIN}/test.h
  ${OPLIBMAIN}/thresh.cpp
  ${OPLIBMAIN}/thresh.h
  ${OPLIBMAIN}/wmra.cpp
  ${OPLIBMAIN}/wmra.h
  ${OPLIBMAIN}/wvthrsh.cpp
  ${OPLIBMAIN}/wvthrsh.h
  ${OPLIBMAIN}/zero.cpp
  ${OPLIBMAIN}/zero.h
)
target_include_directories(oplib PRIVATE
  ${SRC}/riplmain
  ${SRC}/ripltool
)
if(MSVC)
  target_link_libraries(oplib
    riplmain
    ripltool
  )
else()
  target_link_libraries(oplib
    riplmain
    ripltool
    m
  )
endif()

add_library(oplib-test-objs OBJECT
  ${CMAKE_CURRENT_BINARY_DIR}/config.h
  ${OPLIBTEST}/AddTest.cpp
)
target_include_directories(oplib-test-objs PRIVATE
  ${OPLIBMAIN}
  ${SRC}/riplmain
  ${SRC}/testlib/public
)

set_target_properties(
  oplib-test-objs
  oplib
  PROPERTIES FOLDER riplmain
)

