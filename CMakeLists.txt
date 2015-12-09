include(CheckCXXCompilerFlag)
include(CheckCXXSourceCompiles)
include(CheckFunctionExists)

cmake_minimum_required(VERSION 2.8)
project(Ripl)

# https://code.google.com/p/rstm/source/browse/trunk/cmake/modules/AddTargetDefinitions.cmake?r=58
function(add_target_definitions target)
  get_target_property(defines ${target} COMPILE_DEFINITIONS)
  if(defines MATCHES "NOTFOUND")
    set(defines "")
  endif()
  foreach(define ${defines} ${ARGN})
    list(APPEND new_defines ${define})
  endforeach()
  set_target_properties(${target} PROPERTIES COMPILE_DEFINITIONS "${new_defines}")
endfunction()

# BEGIN: Compiler configuration

check_function_exists(strdup HAVE_STRDUP)
check_function_exists(strlwr HAVE_STRLWR)
check_function_exists(strupr HAVE_STRUPR)

# Determine if std::make_unique is supported
set(CMAKE_REQUIRED_FLAGS ${CMAKE_CXX_FLAGS})
check_cxx_source_compiles("#include <memory>\nclass C { };\nint main() { std::make_unique<C>(); }" HAVE_MAKE_UNIQUE)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/config.h
)

if(MSVC)
  set(WARNING_FLAGS "")
  set(WARNING_FLAGS "${WARNING_FLAGS} -wd4018") # "signed/unsigned mismatch"
  set(WARNING_FLAGS "${WARNING_FLAGS} -wd4244") # "conversion from 'double' to 'int', possible loss of data"
  set(WARNING_FLAGS "${WARNING_FLAGS} -wd4305") # "truncation from 'double' to 'float'"
  set(WARNING_FLAGS "${WARNING_FLAGS} -wd4838") # "conversion from 'double' to 'float' requires a narrowing conversion"
  set(WARNING_FLAGS "${WARNING_FLAGS} -wd4996") # "This function or variable may be unsafe"
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNING_FLAGS}")
else()
  # Detect C++11 support and warn if (possibly) absent
  check_cxx_compiler_flag("-std=c++11" COMPILER_SUPPORTS_CXX11)
  check_cxx_compiler_flag("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
  if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
  else()
    message(FATAL_ERROR "Failed to detect C++11 mode for compiler ${CMAKE_CXX_COMPILER}")
  endif()
endif(MSVC)

# END: Compiler configuration

set(SOURCEDIR src)
get_filename_component(TESTSOURCEDIR ${SOURCEDIR}/test ABSOLUTE)

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${SOURCEDIR}/oplib
  ${SOURCEDIR}/riplmain
  ${SOURCEDIR}/ripltool
  third-party/catch/single_include
)

add_library(
  oplib
  ${SOURCEDIR}/oplib/add.cpp
  ${SOURCEDIR}/oplib/add.h
  ${SOURCEDIR}/oplib/ahe.cpp
  ${SOURCEDIR}/oplib/ahe.h
  ${SOURCEDIR}/oplib/binmorph.cpp
  ${SOURCEDIR}/oplib/binmorph.h
  ${SOURCEDIR}/oplib/carve.cpp
  ${SOURCEDIR}/oplib/carve.h
  ${SOURCEDIR}/oplib/conv.cpp
  ${SOURCEDIR}/oplib/conv.h
  ${SOURCEDIR}/oplib/dllconv.cpp
  ${SOURCEDIR}/oplib/dllconv.h
  ${SOURCEDIR}/oplib/fftfilt.cpp
  ${SOURCEDIR}/oplib/fftfilt.h
  ${SOURCEDIR}/oplib/gauss.cpp
  ${SOURCEDIR}/oplib/gauss.h
  ${SOURCEDIR}/oplib/glmorph.cpp
  ${SOURCEDIR}/oplib/glmorph.h
  ${SOURCEDIR}/oplib/globalhe.cpp
  ${SOURCEDIR}/oplib/globalhe.h
  ${SOURCEDIR}/oplib/magsynth.cpp
  ${SOURCEDIR}/oplib/magsynth.h
  ${SOURCEDIR}/oplib/marrhild.cpp
  ${SOURCEDIR}/oplib/marrhild.h
  ${SOURCEDIR}/oplib/mask.cpp
  ${SOURCEDIR}/oplib/mask.h
  ${SOURCEDIR}/oplib/median.cpp
  ${SOURCEDIR}/oplib/median.h
  ${SOURCEDIR}/oplib/misc.cpp
  ${SOURCEDIR}/oplib/misc.h
  ${SOURCEDIR}/oplib/nop.cpp
  ${SOURCEDIR}/oplib/nop.h
  ${SOURCEDIR}/oplib/phsynth.cpp
  ${SOURCEDIR}/oplib/phsynth.h
  ${SOURCEDIR}/oplib/plane.cpp
  ${SOURCEDIR}/oplib/plane.h
  ${SOURCEDIR}/oplib/siahe.cpp
  ${SOURCEDIR}/oplib/siahe.h
  ${SOURCEDIR}/oplib/siahe2.cpp
  ${SOURCEDIR}/oplib/siahe2.h
  ${SOURCEDIR}/oplib/sobel.cpp
  ${SOURCEDIR}/oplib/sobel.h
  ${SOURCEDIR}/oplib/spectrum.cpp
  ${SOURCEDIR}/oplib/spectrum.h
  ${SOURCEDIR}/oplib/test.cpp
  ${SOURCEDIR}/oplib/test.h
  ${SOURCEDIR}/oplib/thresh.cpp
  ${SOURCEDIR}/oplib/thresh.h
  ${SOURCEDIR}/oplib/wmra.cpp
  ${SOURCEDIR}/oplib/wmra.h
  ${SOURCEDIR}/oplib/wvthrsh.cpp
  ${SOURCEDIR}/oplib/wvthrsh.h
  ${SOURCEDIR}/oplib/zero.cpp
  ${SOURCEDIR}/oplib/zero.h
)
if(MSVC)
  target_link_libraries(
    oplib
    riplmain
    ripltool
  )
else()
  target_link_libraries(
    oplib
    riplmain
    ripltool
    m
  )
endif()

add_library(
  riplmain
  ${SOURCEDIR}/riplmain/ripl.h
  ${SOURCEDIR}/riplmain/riplallc.cpp
  ${SOURCEDIR}/riplmain/riplallc.h
  ${SOURCEDIR}/riplmain/ripldbug.h
  ${SOURCEDIR}/riplmain/ripldefs.h
  ${SOURCEDIR}/riplmain/riplfilt.cpp
  ${SOURCEDIR}/riplmain/riplfilt.h
  ${SOURCEDIR}/riplmain/riplgrey.h
  ${SOURCEDIR}/riplmain/riplimg.h
  ${SOURCEDIR}/riplmain/riplmisc.cpp
  ${SOURCEDIR}/riplmain/riplmisc.h
  ${SOURCEDIR}/riplmain/riplmsg.cpp
  ${SOURCEDIR}/riplmain/riplmsg.h
  ${SOURCEDIR}/riplmain/riplpars.cpp
  ${SOURCEDIR}/riplmain/riplpars.h
  ${SOURCEDIR}/riplmain/riplpbm.cpp
  ${SOURCEDIR}/riplmain/riplpbm.h
  ${SOURCEDIR}/riplmain/riplrgb.cpp
  ${SOURCEDIR}/riplmain/riplrgb.h
)

add_library(
  riplop
  ${SOURCEDIR}/riplop/riplentr.cpp
  ${SOURCEDIR}/riplop/riplop.cpp
  ${SOURCEDIR}/riplop/riplop.h
  ${SOURCEDIR}/riplop/riplreg.cpp
)
target_link_libraries(
  riplop
  oplib
)

add_library(
  ripltool
  ${SOURCEDIR}/ripltool/riplfft.cpp
  ${SOURCEDIR}/ripltool/riplfft.h
  ${SOURCEDIR}/ripltool/riplrand.cpp
  ${SOURCEDIR}/ripltool/riplrand.h
  ${SOURCEDIR}/ripltool/ripltdfs.h
  ${SOURCEDIR}/ripltool/riplwave.cpp
  ${SOURCEDIR}/ripltool/riplwave.h
)

add_executable(
  ripl
  ${SOURCEDIR}/riplcmd/riplcmd.cpp
)
target_link_libraries(
  ripl
  oplib
  riplmain
  riplop
  ripltool
)

add_executable(
  ripltest
  ${TESTSOURCEDIR}/ImageTest.cpp
  ${TESTSOURCEDIR}/data.cpp
  ${TESTSOURCEDIR}/data.h
  ${TESTSOURCEDIR}/main.cpp
)
if(NOT NO_USE_CATCH_MAIN)
  add_target_definitions(ripltest USE_CATCH_MAIN)
endif()
target_link_libraries(
  ripltest
  oplib
  riplmain
  riplop
  ripltool
)
