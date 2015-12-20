set(RIPLREGISTRYMAIN ${SRC}/riplregistry/main)
set(RIPLREGISTRYPUBLIC ${SRC}/riplregistry/public)
set(RIPLREGISTRYTEST ${SRC}/riplregistry/test)

add_library(riplregistry-objs OBJECT
  ${CMAKE_CURRENT_BINARY_DIR}/generated/config.h
  ${RIPLREGISTRYMAIN}/Registry.cpp
  ${RIPLREGISTRYPUBLIC}/riplregistry/OpFunc.h
  ${RIPLREGISTRYPUBLIC}/riplregistry/Registry.h
  ${RIPLREGISTRYPUBLIC}/riplregistry/defs.h
)
target_compile_definitions(riplregistry-objs PRIVATE
  BUILD_RIPLREGISTRY
)
target_include_directories(riplregistry-objs PRIVATE
  ${CMAKE_CURRENT_BINARY_DIR}/generated
  ${RIPLREGISTRYPUBLIC}
  ${SRC}/shared
)

add_library(riplregistry-test-objs OBJECT
  ${CMAKE_CURRENT_BINARY_DIR}/generated/config.h
  ${RIPLREGISTRYTEST}/OpFuncTest.cpp
  ${RIPLREGISTRYTEST}/RegistryTest.cpp
)
target_compile_definitions(riplregistry-test-objs PRIVATE
  BUILD_RIPLREGISTRY
)
target_include_directories(riplregistry-test-objs PRIVATE
  ${CMAKE_CURRENT_BINARY_DIR}/generated
  ${RIPLREGISTRYPUBLIC}
  ${SRC}/shared
)

add_library(riplregistry SHARED
  $<TARGET_OBJECTS:riplregistry-objs>
)

set_target_properties(
  riplregistry-objs
  riplregistry-test-objs
  riplregistry
  PROPERTIES FOLDER riplregistry
)

