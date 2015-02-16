cmake_minimum_required(VERSION 2.8)
project(Ripl)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -v -stdlib=libc++")

set(SOURCEDIR src)

include_directories(
  ${SOURCEDIR}/oplib
  ${SOURCEDIR}/riplmain
  ${SOURCEDIR}/ripltool
)

file(
  GLOB
  OPLIBSOURCES
  ${SOURCEDIR}/oplib/*.c
  ${SOURCEDIR}/oplib/*.cpp
  ${SOURCEDIR}/oplib/*.h
)
add_library(
  oplib
  ${OPLIBSOURCES}
)
target_link_libraries(
  oplib
  riplmain
  ripltool
  m
)

file(
  GLOB
  RIPLMAINSOURCES
  ${SOURCEDIR}/riplmain/*.c
  ${SOURCEDIR}/riplmain/*.cpp
  ${SOURCEDIR}/riplmain/*.h
)
add_library(
  riplmain
  ${RIPLMAINSOURCES}
)

file(
  GLOB
  RIPLOPSOURCES
  ${SOURCEDIR}/riplop/*.c
  ${SOURCEDIR}/riplop/*.cpp
  ${SOURCEDIR}/riplop/*.h
)
add_library(
  riplop
  ${RIPLOPSOURCES}
)
target_link_libraries(
  riplop
  oplib
)

file(
  GLOB
  RIPLTOOLSOURCES
  ${SOURCEDIR}/ripltool/*.c
  ${SOURCEDIR}/ripltool/*.cpp
  ${SOURCEDIR}/ripltool/*.h
)
add_library(
  ripltool
  ${RIPLTOOLSOURCES}
)

file(
  GLOB
  RIPLCMDSOURCES
  ${SOURCEDIR}/riplcmd/*.c
  ${SOURCEDIR}/riplcmd/*.cpp
  ${SOURCEDIR}/riplcmd/*.h
)
add_executable(
  ripl
  ${RIPLCMDSOURCES}
)
target_link_libraries(
  ripl
  oplib
  riplmain
  riplop
  ripltool
)

