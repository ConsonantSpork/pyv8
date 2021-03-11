#include "fixtures.hpp"

using namespace pyv8_tests;

v8::Platform* V8Init::platform;

void V8Init::SetUpTestCase() {
    V8Init::platform = v8::platform::NewDefaultPlatform().release();
    v8::V8::InitializePlatform(V8Init::platform);
    v8::V8::Initialize();
}

void V8Init::TearDownTestCase() {
    v8::V8::Dispose();
    v8::V8::ShutdownPlatform();
}
