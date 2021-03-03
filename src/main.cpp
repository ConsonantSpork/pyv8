#include <iostream>
#include "v8_instance.hpp"

using namespace v8;

int main(int argc, char** argv) {
    std::unique_ptr<v8::Platform> platform = platform::NewDefaultPlatform();
    V8::InitializePlatform(platform.get());
    V8::Initialize();
    {
        pyv8::V8Instance instance;
        auto res = instance.run_source("'Hello, ' + 'World!'");
        std::cout << res << std::endl;
    }

    {
        pyv8::V8Instance instance2;
        auto res = instance2.run_source("'Hello, ' + 'World1!'");
        std::cout << res << std::endl;
    }

    V8::Dispose();
    V8::ShutdownPlatform();
   
    return 0;
}
