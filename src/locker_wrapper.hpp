#pragma once

#include <v8.h>
#include <boost/python.hpp>
#include "v8_instance.hpp"

using namespace v8;
namespace bpy = boost::python;

namespace pyv8 {
    /**
     * \brief Wraps v8::Locker class for use in Python
     *
     * This class is a context manager which locks access to a V8Instance. You do not
     * have to use it if you do not plan to run V8 in a multi-threaded environment. However,
     * if you do use this class to call methods on a V8Instance, all following calls to V8Instance
     * must acquire this lock.
     *
     * \warning Once this context manager has been entered, all following calls to V8Instance objects must be
     *  also locked, otherwise V8 will crash.
     **/
    class LockerWrapper {
        private:
            const V8Instance& instance;
            Locker* locker;

        public:
            /**
             * Constructs LockerWrapper from V8Instance
             *
             * @param instance V8Instance object to lock later
             */
            LockerWrapper(const V8Instance& instance);

            /**
             * \brief Enter context manager
             *
             * Calling this method ensures that only one thread can access this isolate
             *
             * @param self python object of type LockerWrapper
             */
            static bpy::object enter(bpy::object self);

            /**
             * \brief Exit context manager
             *
             * Exit context manager and unlock V8Instance to be used in other threads
             *
             * @param self python object of type LockerWrapper
             * @param exc_type type of exception thrown inside the body of this context manager
             * @param exc_value value of exception thrown inside the body of this context manager
             */
            bool exit(bpy::object self, bpy::object exc_type, bpy::object exc_value);
    };
}
