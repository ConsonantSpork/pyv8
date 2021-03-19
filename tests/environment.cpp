#include <iostream>
#include <boost/python.hpp>
#include "environment.hpp"
#include "../src/v8_initializer.hpp"

using namespace pyv8_tests;

void V8Environment::SetUp() {
    Py_Initialize();
    pyv8::V8Initializer::getInstance();
}

void V8Environment::TearDown() {
    pyv8::V8Initializer::getInstance().cleanup();
}
