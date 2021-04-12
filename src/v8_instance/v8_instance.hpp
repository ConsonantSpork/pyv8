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
            std::vector<Persistent<Context, CopyablePersistentTraits<Context>>>* contexts;
            size_t current_ctx;

            Local<Context> get_current_ctx();

            std::string catch_exception(TryCatch&);
            Local<Value> run_source(std::string, Local<Context>);

            template <typename T>
            bpy::object v8_local_to_py_object(Local<Value>,
                                              MaybeLocal<T> (Value::*)(Local<Context>) const,
                                              Local<Context>);
            bpy::object v8_local_to_py_object(Local<Object>, Local<Context>);
            bpy::object v8_local_to_py_object(Local<Array>, Local<Context>);
            bpy::object v8_local_to_py_object(Local<String>, Local<Context>);
            bpy::object v8_local_to_py_object(Local<BigInt>, Local<Context>);
            bpy::object convert(Local<Value>, Local<Context>);

        public:
            V8Instance();
            ~V8Instance();

            /** Get v8::Isolate wrapped by this instance */
            Isolate* get_isolate() const;

            /**
             * Run JavaScript source string
             *
             * @param src JavaScript source to run
             * @throw V8Exception on compile/runtime JavaScript error
             * @retuns boost::python::object wrapping result of evaluating src
             */
            bpy::object run_source(std::string src);

            size_t create_new_context();
            void set_context(size_t context_id);
            bpy::object get_context_ids();
            bpy::object get_current_ctx_id();
    };
}