#pragma once

#include <string>
#include <v8.h>
#include <libplatform/libplatform.h>
#include <libplatform/libplatform-export.h>
#include <libplatform/v8-tracing.h>
#include <boost/python.hpp>

using namespace v8;
namespace bpy = boost::python;

namespace pyv8 {
    class V8Instance {
        private:
            Isolate* isolate;
            Isolate::CreateParams* create_params;

            std::string catch_exception(TryCatch&);
            Local<Value> run_source(std::string, Local<Context>);

        public:
            V8Instance();
            ~V8Instance();
            bpy::object py_run_source(std::string);
    };

    template <typename T>
    bpy::object v8_local_to_py_object(Local<Value>,
                                      MaybeLocal<T> (Value::*)(Local<Context>) const,
                                      Local<Context>);

    bpy::object convert(Local<Value>, Local<Context>);
}
