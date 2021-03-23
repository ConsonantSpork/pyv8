#include <v8_instance.hpp>
#include <gtest/gtest.h>
#include "instance_fixture.hpp"
#include "../src/v8_exception.hpp"

namespace pyv8_tests {
    class ExceptionHandling : public V8InstanceFixture {};

    TEST_F(ExceptionHandling, CompiletimeExCaught) {
        EXPECT_THROW(instance->run_source("break"), pyv8::V8Exception);
    }

    TEST_F(ExceptionHandling, RuntimeExCaught) {
        EXPECT_THROW(instance->run_source("variable"), pyv8::V8Exception);
    }
}
