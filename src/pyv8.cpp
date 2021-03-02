// #include <iostream>
#include <boost/python.hpp>
#include "v8_instance.hpp"
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <v8.h>
// #include <libplatform/libplatform.h>
// #include <libplatform/libplatform-export.h>
// #include <libplatform/v8-tracing.h>

// v8::String::Utf8Value* run_source(v8::Isolate* isolate, const char* source) {
//   v8::Isolate::Scope isolate_scope(isolate);
//   v8::HandleScope handle_scope(isolate);
//   v8::Local<v8::Context> context = v8::Context::New(isolate);
//   v8::Context::Scope context_scope(context);

//   v8::Local<v8::String> v8_source =
//     v8::String::NewFromUtf8(isolate, source)
//     .ToLocalChecked();
//   v8::MaybeLocal<v8::Script> script_dirty =
//     v8::Script::Compile(context, v8_source);
//   v8::Local<v8::Script> script;
//   if (!script_dirty.ToLocal(&script))
//     return nullptr;

//   v8::MaybeLocal<v8::Value> result_dirty = script->Run(context);
//   v8::Local<v8::Value> result_checked;
//   return result_dirty.ToLocal(&result_checked) ?
//     new v8::String::Utf8Value(isolate, result_checked) :
//     nullptr;
// }

// std::string py_run_source(const char* source) {
//   // v8::V8::InitializeICUDefaultLocation(argv[0]);
//   // v8::V8::InitializeExternalStartupData(argv[0]);
//   std::unique_ptr<v8::Platform> platform = v8::platform::NewDefaultPlatform();
//   v8::V8::InitializePlatform(platform.get());
//   v8::V8::Initialize();
//   v8::Isolate::CreateParams create_params;
//   create_params.array_buffer_allocator =
//       v8::ArrayBuffer::Allocator::NewDefaultAllocator();
//   v8::Isolate* isolate = v8::Isolate::New(create_params);

//   v8::String::Utf8Value* res = run_source(isolate, source);
//   res = run_source(isolate, source);
//   if(res == nullptr) {
//     return std::string("Error!");
//   }
//   std::string ret(**res);
//   delete res;
//   isolate->Dispose();
//   v8::V8::Dispose();
//   v8::V8::ShutdownPlatform();
//   delete create_params.array_buffer_allocator;
//   return ret;
// }

BOOST_PYTHON_MODULE(_pyv8)
{
  boost::python::class_<pyv8::V8Instance>("V8Instance")
    .def("run_source", &pyv8::V8Instance::run_source);
}
