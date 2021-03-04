#include <v8_instance.hpp>
#include <gtest/gtest.h>
#include "fixtures.hpp"

namespace pyv8_tests {
    TEST_F(V8Init, CompiletimeExCaught) {
        pyv8::V8Instance instance;
        EXPECT_EQ(instance.run_source("break"), "Uncaught SyntaxError: Illegal break statement");
    }

    TEST_F(V8Init, RuntimeExCaught) {
        pyv8::V8Instance instance;
        EXPECT_EQ(instance.run_source("variable"), "Uncaught ReferenceError: variable is not defined");
    }
}
