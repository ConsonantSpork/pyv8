add_executable(gtest
        main.cpp
        exception_handling.cpp
        conversion.cpp
        environment.hpp
        instance_fixture.hpp
        circular_references.cpp
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
        v8_instance
        ${Boost_LIBRARIES}
        ${GTEST_LIBRARIES}
        ${Python3_LIBRARIES}
        )

enable_testing()
gtest_add_tests(TARGET gtest)
