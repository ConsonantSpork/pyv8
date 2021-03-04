#include <v8_instance.hpp>
#include <gtest/gtest.h>
#include "fixtures.hpp"

namespace pyv8_tests {
    TEST_F(V8Init, SourceRuns) {
        pyv8::V8Instance instance;
        EXPECT_EQ(instance.run_source("'Hello, ' + 'World!'"), "Hello, World!");
    }
}
