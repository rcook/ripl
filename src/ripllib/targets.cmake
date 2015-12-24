set(RIPLLIBMAIN ${SRC}/ripllib/main)
set(RIPLLIBPUBLIC ${SRC}/ripllib/public)
set(RIPLLIBTEST ${SRC}/ripllib/test)

add_library(ripllib STATIC
  ${SHAREDSRCS}
  ${RIPLLIBMAIN}/Registry.cpp
  ${RIPLLIBPUBLIC}/ripllib/ExecuteFunc.h
  ${RIPLLIBPUBLIC}/ripllib/HelpFunc.h
  ${RIPLLIBPUBLIC}/ripllib/Registry.h
)

target_compile_definitions(ripllib PRIVATE BUILD_RIPLLIB)

target_include_directories(ripllib PRIVATE
  ${RIPLLIBPUBLIC}
  ${SRC}/riplmain
)

add_library(ripllib-test-objs OBJECT
  ${SHAREDSRCS}
  ${RIPLLIBTEST}/RegistryTest.cpp
)

target_compile_definitions(ripllib-test-objs PRIVATE BUILD_RIPLLIB)

target_include_directories(ripllib-test-objs PRIVATE
  ${RIPLLIBPUBLIC}
  ${SRC}/riplmain
)

set_target_properties(
  ripllib-test-objs
  ripllib
  PROPERTIES FOLDER ripllib
)
