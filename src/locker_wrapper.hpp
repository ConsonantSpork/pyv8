#pragma once

#include <v8.h>
#include <boost/python.hpp>
#include "v8_instance.hpp"

using namespace v8;
namespace bpy = boost::python;

namespace pyv8 {
    class LockerWrapper {
        private:
            const V8Instance& instance;
            Locker* locker;

        public:
            LockerWrapper(const V8Instance&);
            static bpy::object enter(bpy::object);
            bool exit(bpy::object, bpy::object, bpy::object);
    };
}
