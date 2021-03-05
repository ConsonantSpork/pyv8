#include "v8_initializer.hpp"
#include "v8_instance.hpp"

using namespace v8;

int main(int argc, char** argv) {
    pyv8::V8Initializer& init = pyv8::V8Initializer::getInstance();
    {
        pyv8::V8Instance instance;
        auto res = instance.run_source("'Hello, ' + 'World!'");
    }

    {
        pyv8::V8Instance instance2;
        auto res = instance2.run_source("'Hello, ' + 'World1!'");
    }
    init.cleanup();
    return 0;
}
