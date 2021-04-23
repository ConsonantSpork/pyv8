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
    /**
     * \brief Provides methods to run JavaScript code
     *
     * Different object of this class can safely be used in different
     * threads. If you are looking to use the same V8Instance in different
     * threads, it has to be synchronized using V8Locker (for python) or
     * v8::Locker (for c++).
     */
    class V8Instance {
        private:
            Isolate* isolate;
            Isolate::CreateParams* create_params;

            Local<Value> run_source(const std::string&);

            template <typename T>
            bpy::object v8_local_to_py_object(Local<Value> value,
                                              MaybeLocal<T> (Value::*ToType)(Local<Context>) const) {
                Value* unwrapped = *value;
                MaybeLocal<T> converted = (unwrapped->*ToType)(isolate->GetCurrentContext());
                Local<T> checked;
                if(!converted.ToLocal(&checked)) {
                    return bpy::object();
                }
                return bpy::object(checked->Value());
            }
            bpy::object v8_local_to_py_object(Local<Object>);
            bpy::object v8_local_to_py_object(Local<Array>);
            bpy::object v8_local_to_py_object(Local<TypedArray>);
            bpy::object v8_local_to_py_object(Local<ArrayBuffer>);
            bpy::object v8_local_to_py_object(Local<String>);
            bpy::object v8_local_to_py_object(Local<BigInt>);
            bpy::object convert(Local<Value>);

        public:
            V8Instance();
            ~V8Instance();

            /** Get v8::Isolate wrapped by this instance */
            Isolate* get_isolate() const;

            /**
             * Run JavaScript source string. If there were no previous calls
             * to V8Instance::new_context, new context will be created and
             * entered implicitly.
             *
             * @param src JavaScript source to run
             * @throw V8Exception on compile/runtime JavaScript error
             * @returns boost::python::object wrapping result of evaluating src
             */
            bpy::object run(const std::string& src);

            /**
             * Create and enter new v8::Context. It does not share its global object with previously entered
             * context(s). You can return to previously entered context by calling V8Instance::exit_context.
             */
            void new_context();

            /**
             * Exit currently entered context and return to prevoius one.
             */
            void exit_context();
    };
}
