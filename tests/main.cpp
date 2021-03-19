#include <v8_instance.hpp>
#include <gtest/gtest.h>
#include "environment.hpp"
#include "../src/v8_initializer.hpp"

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    testing::AddGlobalTestEnvironment(new pyv8_tests::V8Environment());
    return RUN_ALL_TESTS();
}
