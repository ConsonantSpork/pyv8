cmake_minimum_required(VERSION 3.15)
project(pyv8 VERSION 0.1)

set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(PythonLibs 3.9 REQUIRED)
find_package(GTest)
find_package(Doxygen)

add_subdirectory(${PROJECT_SOURCE_DIR}/src/v8_instance)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/pyv8)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/main)
if(${GTest_FOUND})
  add_subdirectory(${PROJECT_SOURCE_DIR}/tests)
endif()
if(DOXYGEN_FOUND)
  add_subdirectory(${PROJECT_SOURCE_DIR}/doc)
endif()
