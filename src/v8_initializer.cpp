#include <libplatform/libplatform.h>
#include <boost/python.hpp>
#include "v8_initializer.hpp"

using namespace pyv8;
namespace bpy = boost::python;

std::unique_ptr<v8::Platform> V8Initializer::platform = v8::platform::NewDefaultPlatform();

V8Initializer& V8Initializer::getInstance() {
    static V8Initializer init;
    return init;
}

V8Initializer::V8Initializer() {
    v8::V8::InitializePlatform(platform.get());
    v8::V8::Initialize();
}

void V8Initializer::cleanup() {
    v8::V8::Dispose();
    v8::V8::ShutdownPlatform();
    platform.reset();
}
