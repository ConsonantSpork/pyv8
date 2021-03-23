#pragma once

#include <gtest/gtest.h>
#include <v8_instance.hpp>
#include <v8.h>
#include "../src/v8_initializer.hpp"

namespace pyv8_tests {
    class V8Environment : public testing::Environment {
        protected:
            void SetUp() {
                Py_Initialize();
                pyv8::V8Initializer::getInstance();
            }
            void TearDown() {
                pyv8::V8Initializer::getInstance().cleanup();
            }
    };
}
