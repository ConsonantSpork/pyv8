#include <iostream>
#include "v8_instance.hpp"

int main(int argc, char** argv) {
    pyv8::V8Instance instance;
    auto res = instance.run_source("error");
    std::cout << res << std::endl;
    return 0;
}
