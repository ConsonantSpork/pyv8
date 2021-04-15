#include <boost/python.hpp>
#include "v8_initializer.hpp"
#include "v8_instance.hpp"
#include "v8_exception.hpp"
#include "locker_wrapper.hpp"

namespace py = boost::python;

PyObject* v8_exception_type = nullptr;

void translate_v8_exception(const pyv8::V8Exception& exc){
  PyObject* args = Py_BuildValue("ssis",
                                 exc.msg().c_str(),
                                 exc.file().c_str(),
                                 exc.lineno(),
                                 exc.func().c_str());
  PyObject* v8_exception_instance = PyObject_Call(v8_exception_type, args, NULL);
  Py_INCREF(v8_exception_instance);

  PyObject* message = PyUnicode_FromString(exc.msg().c_str());
  PyObject* file = PyUnicode_FromString(exc.file().c_str());
  PyObject* lineno = PyLong_FromLong(exc.lineno());
  PyObject* func = PyUnicode_FromString(exc.func().c_str());

  PyObject_SetAttrString(v8_exception_instance, "message", message);
  PyObject_SetAttrString(v8_exception_instance, "file", file);
  PyObject_SetAttrString(v8_exception_instance, "lineno", lineno);
  PyObject_SetAttrString(v8_exception_instance, "func", func);

  PyErr_SetObject(v8_exception_type, v8_exception_instance);
}

BOOST_PYTHON_MODULE(_pyv8)
{
  py::object (pyv8::V8Instance::*run)(std::string) = &pyv8::V8Instance::run;
  py::class_<pyv8::V8Instance>
    ("V8Instance")
    .def("run", run)
    .def("new_context", &pyv8::V8Instance::new_context)
    .def("exit_context", &pyv8::V8Instance::exit_context);

  py::class_<pyv8::V8Initializer, boost::shared_ptr<pyv8::V8Initializer>, boost::noncopyable>
    ("V8Initializer", py::no_init)
    .def("cleanup", &pyv8::V8Initializer::cleanup)
    .def("get_instance", py::make_function(&pyv8::V8Initializer::getInstance,
                                           py::return_value_policy<py::reference_existing_object>()));

  py::class_<pyv8::LockerWrapper>
    ("V8Locker", py::init<pyv8::V8Instance&>())
    .def("__enter__", &pyv8::LockerWrapper::enter)
    .def("__exit__", &pyv8::LockerWrapper::exit);

  v8_exception_type = PyErr_NewException("_pyv8.V8Error", PyExc_Exception, NULL);
  py::scope().attr("V8Error") = py::handle<>(v8_exception_type);

  py::register_exception_translator<pyv8::V8Exception>(translate_v8_exception);
}
