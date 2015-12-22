set(RIPLREGISTRYMAIN ${SRC}/riplregistry/main)
set(RIPLREGISTRYPUBLIC ${SRC}/riplregistry/public)
set(RIPLREGISTRYTEST ${SRC}/riplregistry/test)

add_library(riplregistry STATIC
  ${SHAREDSRCS}
  ${RIPLREGISTRYMAIN}/Registry.cpp
  ${RIPLREGISTRYPUBLIC}/riplregistry/ExecuteFunc.h
  ${RIPLREGISTRYPUBLIC}/riplregistry/HelpFunc.h
  ${RIPLREGISTRYPUBLIC}/riplregistry/Registry.h
)

target_compile_definitions(riplregistry PRIVATE BUILD_RIPLREGISTRY)

target_include_directories(riplregistry PRIVATE
  ${RIPLREGISTRYPUBLIC}
  ${SRC}/riplmain
)

add_library(riplregistry-test-objs OBJECT
  ${SHAREDSRCS}
  ${RIPLREGISTRYTEST}/RegistryTest.cpp
)

target_compile_definitions(riplregistry-test-objs PRIVATE BUILD_RIPLREGISTRY)

target_include_directories(riplregistry-test-objs PRIVATE
  ${RIPLREGISTRYPUBLIC}
  ${SRC}/riplmain
)

set_target_properties(
  riplregistry-test-objs
  riplregistry
  PROPERTIES FOLDER riplregistry
)
