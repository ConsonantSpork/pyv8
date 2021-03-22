#include "locker_wrapper.hpp"

using namespace v8;
namespace bpy = boost::python;

pyv8::LockerWrapper::LockerWrapper(const pyv8::V8Instance& instance) :
    instance(instance),
    locker(nullptr)
{};

bpy::object pyv8::LockerWrapper::enter(bpy::object self) {
    LockerWrapper* myself = bpy::extract<LockerWrapper*>(self);
    myself->locker = new Locker(myself->instance.get_isolate());
    return self;
}

bool pyv8::LockerWrapper::exit(bpy::object type, bpy::object value, bpy::object traceback) {
    delete locker;
    return false;  // Do not suppress exception
}
