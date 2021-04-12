find_package(Threads REQUIRED)
find_package(Boost)
add_library(v8_instance
  STATIC
  v8_instance.cpp
  v8_exception.cpp
  v8_initializer.cpp
  locker_wrapper.cpp
  )
set_target_properties(v8_instance PROPERTIES
  POSITION_INDEPENDENT_CODE ON
  )
target_include_directories(v8_instance
  PUBLIC
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/src
  /usr/include/python3.9
  )
target_link_directories(v8_instance
  PUBLIC
  lib
  )
target_link_libraries(v8_instance
  PRIVATE
  Threads::Threads
  PUBLIC
  ${PROJECT_SOURCE_DIR}/lib/libv8_base_without_compiler.a
  ${PROJECT_SOURCE_DIR}/lib/libv8_compiler.a
  ${PROJECT_SOURCE_DIR}/lib/libv8_inspector.a
  ${PROJECT_SOURCE_DIR}/lib/libv8_initializers.a
  ${PROJECT_SOURCE_DIR}/lib/libv8_libbase.a
  ${PROJECT_SOURCE_DIR}/lib/libv8_libsampler.a
  ${PROJECT_SOURCE_DIR}/lib/libv8_libplatform.a
  ${PROJECT_SOURCE_DIR}/lib/libv8_snapshot.a
  ${PROJECT_SOURCE_DIR}/lib/libv8_torque_generated.a
  ${PROJECT_SOURCE_DIR}/lib/libv8_base_without_compiler.a # FIXME: This is hacky
  )
