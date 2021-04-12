add_executable(gtest
  main.cpp
  exception_handling.cpp
  conversion.cpp
  )
get_target_property(V8_INSTANCE_SOURCE_DIR v8_instance SOURCE_DIR)
target_include_directories(gtest
  PRIVATE
  ${GTest_INCLUDE_DIRS}
  ${V8_INSTANCE_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/include
  )
target_link_libraries(gtest
  PRIVATE
  ${Boost_LIBRARIES}
  boost_python39
  ${PYTHON_LIBRARIES}
  v8_instance
  ${GTEST_LIBRARIES}
  )

enable_testing()
gtest_add_tests(TARGET gtest)
