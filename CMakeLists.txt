cmake_minimum_required(VERSION 3.13)
project(pyv8 VERSION 0.1)

option(MODULE_ONLY "Build python module only" OFF)

set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Python3 3.7 REQUIRED COMPONENTS Development)
find_package(Boost REQUIRED COMPONENTS python)
find_package(GTest)
find_package(Doxygen)

add_subdirectory(${PROJECT_SOURCE_DIR}/src/v8_instance)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/pyv8)
if(NOT ${MODULE_ONLY})
  add_subdirectory(${PROJECT_SOURCE_DIR}/src/main)
  if(${GTest_FOUND})
    add_subdirectory(${PROJECT_SOURCE_DIR}/tests)
  endif()
  if(DOXYGEN_FOUND)
    add_subdirectory(${PROJECT_SOURCE_DIR}/doc)
  endif()
endif()

install(TARGETS pyv8
  LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}
  RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}
  )
