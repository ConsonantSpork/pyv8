#include <boost/python.hpp>
#include "v8_initializer.hpp"
#include "v8_instance.hpp"
#include "v8_exception.hpp"

namespace py = boost::python;

PyObject* v8_exception_type = nullptr;

void translate_v8_exception(const pyv8::V8Exception& exc){
  PyObject* message = PyUnicode_FromString(exc.what());
  PyErr_SetObject(v8_exception_type, message);
}

BOOST_PYTHON_MODULE(_pyv8)
{
  py::object (pyv8::V8Instance::*run_source)(std::string) = &pyv8::V8Instance::run_source;
  py::class_<pyv8::V8Instance>
    ("V8Instance")
    .def("run_source", run_source);

  py::class_<pyv8::V8Initializer, boost::shared_ptr<pyv8::V8Initializer>, boost::noncopyable>
    ("V8Initializer", py::no_init)
    .def("cleanup", &pyv8::V8Initializer::cleanup)
    .def("get_instance", py::make_function(&pyv8::V8Initializer::getInstance,
                                           py::return_value_policy<py::reference_existing_object>()));

  v8_exception_type = PyErr_NewException("_pyv8.V8Error", PyExc_Exception, NULL);
  py::register_exception_translator<pyv8::V8Exception>(translate_v8_exception);
}
