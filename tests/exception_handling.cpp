#include <v8_instance.hpp>
#include <gtest/gtest.h>
#include "../src/v8_exception.hpp"

namespace pyv8_tests {
    TEST(Exceptions, CompiletimeExCaught) {
        {
            pyv8::V8Instance instance;
            EXPECT_THROW(instance.run_source("break"), pyv8::V8Exception);
        }
    }

    TEST(Exceptions, RuntimeExCaught) {
        {
            pyv8::V8Instance instance;
            EXPECT_THROW(instance.run_source("variable"), pyv8::V8Exception);
        }
    }
}
