list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/third-party/cmakelib")
include(AddPlatformDefinitions)
include(AddTargetDefinitions)
include(CheckFunctionExists)
include(CheckMakeUniqueExists)
include(RequireCpp11)
include(SetCompilerFlags)

cmake_minimum_required(VERSION 2.8.12)
project(Ripl)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER zzzCMakePredefinedTargets)

set(CMAKE_MACOSX_RPATH 1)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH true)
set(LIB_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/lib)
list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES ${LIB_INSTALL_DIR} isSystemDir)
if("${isSystemDir}" STREQUAL "-1")
  set(CMAKE_INSTALL_RPATH ${LIB_INSTALL_DIR})
endif()

add_platform_definitions()
check_make_unique_exists()
require_cpp11()
set_compiler_flags()
check_function_exists(strdup HAVE_STRDUP)
check_function_exists(strlwr HAVE_STRLWR)
check_function_exists(strupr HAVE_STRUPR)

if(MSVC)
  set(WARNING_FLAGS "")
  set(WARNING_FLAGS "${WARNING_FLAGS} -wd4018") # "signed/unsigned mismatch"
  set(WARNING_FLAGS "${WARNING_FLAGS} -wd4244") # "conversion from 'double' to 'int', possible loss of data"
  set(WARNING_FLAGS "${WARNING_FLAGS} -wd4305") # "truncation from 'double' to 'float'"
  set(WARNING_FLAGS "${WARNING_FLAGS} -wd4838") # "conversion from 'double' to 'float' requires a narrowing conversion"
  set(WARNING_FLAGS "${WARNING_FLAGS} -wd4996") # "This function or variable may be unsafe"
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNING_FLAGS}")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -g")
endif(MSVC)

set(SRC ${CMAKE_CURRENT_SOURCE_DIR}/src)

configure_file(
  ${SRC}/config.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/config.h
)

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  third-party/catch/single_include
)

include(src/oplib/targets.cmake)
include(src/ripl/targets.cmake)
include(src/riplmain/targets.cmake)
include(src/riplop/targets.cmake)
include(src/ripltool/targets.cmake)
include(src/testlib/targets.cmake)

include(src/app/targets.cmake)
include(src/interfacelib/targets.cmake)
include(src/plugin/targets.cmake)
include(src/unittest/targets.cmake)
include(src/utillib/targets.cmake)

install(TARGETS
  app
  interfacelib
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
)
install(TARGETS
  plugin
  DESTINATION bin/plugins
)

enable_testing()
add_test(NAME unittest COMMAND unittest)

