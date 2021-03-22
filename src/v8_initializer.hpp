#pragma once

#include <v8.h>

namespace pyv8 {
    /**
     * \brief Performs initialization necessary for V8 to function properly
     */
    class V8Initializer {
        private:
            static std::unique_ptr<v8::Platform> platform;
            V8Initializer();
            V8Initializer(V8Initializer const&) = delete;
            V8Initializer& operator= (V8Initializer const&) = delete;
        public:
            /**
             * \brief Initializes V8
             *
             * @returns A reference to V8Initializer to allow user to
             * perform clean up at a later point. However, if V8 will be used until
             * termination of program, return value can be safely ignored.
             */
            static V8Initializer& getInstance();

            /**
             * \brief Shutdown V8
             *
             * Free all resources used by V8. This should only be called once,
             * multiple calls will cause V8 to crash the program.
             *
             * \warning After this method had been called calls to V8Instance or to
             * V8Initializer#getInstance() will crash the program. Only call this method
             * to shutdown V8 when you no longer plan to use it.
             */
            void cleanup();
    };
}
