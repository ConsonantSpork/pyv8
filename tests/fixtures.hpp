#pragma once

#include <gtest/gtest.h>
#include <v8_instance.hpp>
#include <v8.h>

namespace pyv8_tests {
    class V8Init : public testing::Test {
        protected:
            static v8::Platform* platform;

            static void SetUpTestCase();
            static void TearDownTestCase();
    };
}
