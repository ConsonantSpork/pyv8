#include <v8.h>
#include <boost/python.hpp>
#include "to_python_converter.hpp"

using namespace v8;
namespace bpy = boost::python;

namespace pyv8 {
    PyObject* V8LocalContextPairToPython::convert(std::pair<Local<Value>, Persistent<Context>> pair) {
        auto value = pair.first;
        auto ctx = pair.second;
        // HandleSope handle_scope(ctx.);
    }
}
