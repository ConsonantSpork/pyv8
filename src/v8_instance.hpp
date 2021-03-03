#pragma once

#include <string>
#include <v8.h>
#include <libplatform/libplatform.h>
#include <libplatform/libplatform-export.h>
#include <libplatform/v8-tracing.h>

namespace pyv8 {
    class V8Instance {
        private:
            std::string catch_exception(v8::TryCatch& try_catch);

        protected:
            v8::Isolate* isolate;
            v8::Isolate::CreateParams* create_params;

        public:
            V8Instance();
            ~V8Instance();
            std::string run_source(std::string);
    };
}
