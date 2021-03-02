#include <iostream>
#include "v8_instance.hpp"

pyv8::V8Instance::V8Instance() {
    // v8::V8::InitializeICUDefaultLocation(argv[0]);
    // v8::V8::InitializeExternalStartupData(argv[0]);
    this->platform = v8::platform::NewDefaultPlatform().release();
    v8::V8::InitializePlatform(this->platform);
    v8::V8::Initialize();
    this->create_params = new v8::Isolate::CreateParams();
    create_params->array_buffer_allocator =
        v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    this->isolate = v8::Isolate::New(*create_params);
}

pyv8::V8Instance::~V8Instance() {
    this->isolate->Dispose();
    v8::V8::Dispose();
    v8::V8::ShutdownPlatform();
    delete this->create_params->array_buffer_allocator;
    delete this->create_params;
}

std::string pyv8::V8Instance::run_source(std::string source) {
    v8::Isolate::Scope isolate_scope(this->isolate);
    v8::HandleScope handle_scope(isolate);
    v8::Local<v8::Context> context = v8::Context::New(this->isolate);
    v8::Context::Scope context_scope(context);

    v8::Local<v8::String> v8_source =
        v8::String::NewFromUtf8(this->isolate, source.c_str())
        .ToLocalChecked();

    v8::MaybeLocal<v8::Script> script_dirty =
        v8::Script::Compile(context, v8_source);  // FIXME: fails here
    v8::Local<v8::Script> script;
    if (!script_dirty.ToLocal(&script))
        return nullptr;

    v8::MaybeLocal<v8::Value> result_dirty = script->Run(context);
    v8::Local<v8::Value> result_checked;
    if(!result_dirty.ToLocal(&result_checked))
        return nullptr;

    v8::String::Utf8Value res_str(isolate, result_checked);
    std::string result(*res_str);
    return result;
}
