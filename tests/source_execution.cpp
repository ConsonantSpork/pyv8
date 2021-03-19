#include <v8_instance.hpp>
#include <gtest/gtest.h>
#include <boost/python.hpp>

namespace pyv8_tests {
    TEST(HappyPath, SourceRuns) {
        pyv8::V8Instance* instance = new pyv8::V8Instance();
        EXPECT_EQ(instance->run_source("'Hello, World!'"),
                  boost::python::object("Hello, World!"));
        delete instance;
    }
}
