#include <iostream>
#include "v8_instance.hpp"

using namespace v8;

pyv8::V8Instance::V8Instance() {
    this->create_params = new Isolate::CreateParams();
    create_params->array_buffer_allocator =
        ArrayBuffer::Allocator::NewDefaultAllocator();
    this->isolate = Isolate::New(*create_params);
}

pyv8::V8Instance::~V8Instance() {
    isolate->Dispose();
    delete create_params->array_buffer_allocator;
}

std::string pyv8::V8Instance::catch_exception(TryCatch& try_catch) {
    Local<Value> exception = try_catch.Exception();
    Local<String> msg = Exception::CreateMessage(isolate, exception)->Get();
    char* buffer = new char[msg->Utf8Length(isolate) + 1];
    msg->WriteUtf8(isolate, buffer);
    return buffer;
}

std::string pyv8::V8Instance::run_source(std::string source) {
    Isolate::Scope isolate_scope(isolate);
    HandleScope handle_scope(isolate);
    Local<Context> context = Context::New(isolate);
    Context::Scope context_scope(context);

    Local<String> v8_source =
        String::NewFromUtf8(isolate, source.c_str())
        .ToLocalChecked();

    TryCatch try_catch(isolate);

    MaybeLocal<Script> script_dirty =
        Script::Compile(context, v8_source);
    Local<Script> script;
    if (!script_dirty.ToLocal(&script)) {
        return try_catch.HasCaught() ?
            catch_exception(try_catch) :
            "Error running script";
    }

    MaybeLocal<Value> result_dirty = script->Run(context);
    Local<Value> result_checked;
    if(!result_dirty.ToLocal(&result_checked)) {
        return try_catch.HasCaught() ?
            catch_exception(try_catch) :
            "Error running script";
    }

    String::Utf8Value res_str(isolate, result_checked);
    return *res_str;
}
