#include <v8_instance.hpp>
#include <gtest/gtest.h>
#include <iostream>

namespace pyv8_tests {
    class V8InstanceFixture : public ::testing::Test {
        protected:
            pyv8::V8Instance* instance;
            virtual void SetUp() {
                instance = new pyv8::V8Instance();
            }
            virtual void TearDown() {
                delete instance;
            }
    };
}
