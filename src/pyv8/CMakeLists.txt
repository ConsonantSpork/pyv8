add_library(pyv8
  SHARED
  pyv8.cpp
  )
target_include_directories(pyv8
  PRIVATE
  /usr/include/python3.9
  ${Boost_INCLUDE_DIRECTORIES}
  ${PROJECT_SOURCE_DIR}/src/v8_instance
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
