cmake_minimum_required(VERSION 3.15)

project(pyv8)

#
# v8_instance
#

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(Boost)
add_library(v8_instance
  STATIC
  src/v8_instance.cpp
  src/py_converter.cpp
  src/v8_initializer.cpp
  )
set_target_properties(v8_instance PROPERTIES
  POSITION_INDEPENDENT_CODE ON
  )
target_include_directories(v8_instance
  PUBLIC
  include
  src
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
  libv8_base_without_compiler.a
  libv8_compiler.a
  libv8_inspector.a
  libv8_initializers.a
  libv8_libbase.a
  libv8_libsampler.a
  libv8_libplatform.a
  libv8_snapshot.a
  libv8_torque_generated.a
  libv8_base_without_compiler.a # FIXME: This is hacky
  )

#
# pyv8
#

add_library(pyv8
  SHARED
  src/pyv8.cpp
  )
target_include_directories(pyv8
  PRIVATE
  /usr/include/python3.9
  ${Boost_INCLUDE_DIRECTORIES}
  )
target_link_libraries(pyv8
  PRIVATE
  ${Boost_LIBRARIES}
  boost_python39
  v8_instance
  )
set_target_properties(pyv8 PROPERTIES
  PREFIX "_"
  )

#
# main
#
find_package(PythonLibs 3.9 REQUIRED)
add_executable(main
  src/main.cpp
  )
target_link_libraries(main
  ${Boost_LIBRARIES}
  boost_python39
  ${PYTHON_LIBRARIES}
  v8_instance
  )

#
# tests
#

find_package(GTest)
if(${GTest_FOUND})
  add_executable(check
    tests/main.cpp
    tests/fixtures.cpp
    tests/exception_handling.cpp
    tests/source_execution.cpp
    )
  target_include_directories(check
    PRIVATE
    src
    ${GTest_INCLUDE_DIRS}
    )
  target_link_libraries(check
    PRIVATE
    v8_instance
    ${GTEST_LIBRARIES}
    )
endif()
