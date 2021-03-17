#pragma once

#include <v8.h>

namespace pyv8 {
    class V8Initializer {
        private:
            static std::unique_ptr<v8::Platform> platform;
            V8Initializer();
            V8Initializer(V8Initializer const&) = delete;
            V8Initializer& operator= (V8Initializer const&) = delete;
        public:
            static V8Initializer& getInstance();
            void cleanup();
    };
}
