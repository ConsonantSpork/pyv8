#include <boost/python.hpp>
#include "v8_initializer.hpp"
#include "v8_instance.hpp"

namespace py = boost::python;

BOOST_PYTHON_MODULE(_pyv8)
{
  py::class_<pyv8::V8Instance>
    ("V8Instance")
    .def("run_source", &pyv8::V8Instance::run_source);

  py::class_<pyv8::V8Initializer, boost::shared_ptr<pyv8::V8Initializer>, boost::noncopyable>
    ("V8Initializer", py::no_init)
    .def("cleanup", &pyv8::V8Initializer::cleanup)
    .def("get_instance", py::make_function(&pyv8::V8Initializer::getInstance,
                                           py::return_value_policy<py::reference_existing_object>()));
}
