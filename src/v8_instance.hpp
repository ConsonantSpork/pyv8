#pragma once

#include <string>
#include <v8.h>
#include <libplatform/libplatform.h>
#include <libplatform/libplatform-export.h>
#include <libplatform/v8-tracing.h>
#include <boost/python.hpp>

using namespace v8;

namespace pyv8 {
    class V8Instance {
        private:
            std::string catch_exception(TryCatch& try_catch);

        protected:
            Isolate* isolate;
            Isolate::CreateParams* create_params;

        public:
            V8Instance();
            ~V8Instance();
            Local<Value> run_source(std::string);
    };
}
