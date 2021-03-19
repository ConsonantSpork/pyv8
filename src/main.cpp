#include <thread>
#include "v8_initializer.hpp"
#include "v8_instance.hpp"

using namespace v8;

void exec_code(pyv8::V8Instance& instance, std::string code) {
    Locker locker(instance.get_isolate());
    instance.run_source(code);
}

int main(int argc, char** argv) {
    Py_Initialize();

    pyv8::V8Initializer& init = pyv8::V8Initializer::getInstance();
    {
        pyv8::V8Instance instance;
        std::thread t1(exec_code, std::ref(instance), "1 + 2");
        std::thread t2(exec_code, std::ref(instance), "2 + 3");
        t1.join();
        t2.join();
    }
    init.cleanup();
    return 0;
}
