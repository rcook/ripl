set(OPLIBMAIN ${SRC}/oplib/main)
set(OPLIBTEST ${SRC}/oplib/test)

add_library(oplib-objs OBJECT
  ${SHAREDSRCS}
  ${OPLIBMAIN}/add.cpp
  ${OPLIBMAIN}/add.h
  ${OPLIBMAIN}/ahe.cpp
  ${OPLIBMAIN}/ahe.h
  ${OPLIBMAIN}/alloc.h
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
  ${OPLIBMAIN}/register.cpp
  ${OPLIBMAIN}/register.h
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

target_include_directories(oplib-objs PRIVATE
  ${SRC}/riplmain
  ${SRC}/riplregistry/public
  ${SRC}/ripltool
)

add_library(oplib-test-objs OBJECT
  ${CMAKE_CURRENT_BINARY_DIR}/generated/resources.h
  ${SHAREDSRCS}
  ${OPLIBTEST}/AddTest.cpp
  ${OPLIBTEST}/GlobalHETest.cpp
  ${OPLIBTEST}/NopTest.cpp
)

target_include_directories(oplib-test-objs PRIVATE
  ${OPLIBMAIN}
  ${SRC}/riplmain
  ${SRC}/riplregistry/public
  ${SRC}/testlib/public
)

add_library(oplib SHARED
  $<TARGET_OBJECTS:oplib-objs>
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

set_target_properties(
  oplib-objs
  oplib-test-objs
  oplib
  PROPERTIES FOLDER oplib
)
