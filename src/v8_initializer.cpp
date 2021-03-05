#include <iostream>
#include <libplatform/libplatform.h>
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

bpy::object pyv8::convert(v8::Local<v8::Value> value,
                          v8::Local<v8::Context> ctx) {
    // if (value->IsBoolean())
    //     bool res = value->ToBoolean(ctx->GetIsolate())->Value();
    // if (value->IsInt32() || value->IsUint32()) {
    //     int64_t res = value->ToInteger(ctx).ToLocalChecked()->Value();
    //     return bpy::object(res);
    // }
    // else if (value->IsNumber()) {
    //     double res = value->ToNumber(ctx).ToLocalChecked()->Value();
    //     return bpy::object(res);
    // }
    // else if (value->TypeOf(Isolate *))
    // else {
    //     return bpy::object("No known conversion");
    // }
    v8::Local<v8::String> type = value->TypeOf(ctx->GetIsolate());
    v8::String::Utf8Value type_str(ctx->GetIsolate(), type);
    std::string ret(*type_str);
    return bpy::object(ret);
}
