add_library(pyv8
  SHARED
  pyv8.cpp
  )
target_include_directories(pyv8
  PRIVATE
  ${Python3_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRECTORIES}
  ${PROJECT_SOURCE_DIR}/src/v8_instance
  )
target_link_libraries(pyv8
  PRIVATE
  ${Boost_LIBRARIES}
  ${Python3_LIBRARIES}
  v8_instance
  )
set_target_properties(pyv8 PROPERTIES
  PREFIX "_"
  )
