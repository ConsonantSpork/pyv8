#include <boost/python.hpp>
#include "v8_instance.hpp"

BOOST_PYTHON_MODULE(_pyv8)
{
  boost::python::class_<pyv8::V8Instance>("V8Instance")
    .def("run_source", &pyv8::V8Instance::run_source);
}
