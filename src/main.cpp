#include "v8_instance.hpp"

int main(int argc, char** argv) {
    pyv8::V8Instance instance;
    instance.run_source("'Hello, ' + 'World!'");
    return 0;
}
