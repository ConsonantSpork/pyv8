#include <v8_instance.hpp>
#include <gtest/gtest.h>
#include "instance_fixture.hpp"
#include <v8_exception.hpp>

namespace pyv8_tests {
    class ExceptionHandling : public V8InstanceFixture {};

    TEST_F(ExceptionHandling, CompiletimeExCaught) {
        EXPECT_THROW(instance->run("break"), pyv8::V8Exception);
    }

    TEST_F(ExceptionHandling, RuntimeExCaught) {
        EXPECT_THROW(instance->run("variable"), pyv8::V8Exception);
    }
}
