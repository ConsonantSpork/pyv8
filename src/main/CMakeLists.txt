add_executable(main
  main.cpp
  )
get_target_property(V8_INSTANCE_SOURCE_DIR v8_instance SOURCE_DIR)
target_include_directories(main
  PRIVATE
  ${V8_INSTANCE_SOURCE_DIR}
  )
target_link_libraries(main
  ${Boost_LIBRARIES}
  boost_python39
  ${PYTHON_LIBRARIES}
  v8_instance
  )
