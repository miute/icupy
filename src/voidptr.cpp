#include "voidptr.hpp"
#include <memory>

_ConstVoidPtr::_ConstVoidPtr(const py::object &value) {
  if (py::isinstance<py::str>(value)) {
    context_ = std::make_shared<std::string>(value.cast<std::string>());
  } else {
    context_ = value;
  }
}

const char *_ConstVoidPtr::c_str() const {
  if (context_.type() == typeid(std::shared_ptr<std::string>)) {
    auto p = std::any_cast<std::shared_ptr<std::string>>(context_);
    return p->data();
  }
  return nullptr;
}

// TODO: Remove deprecated method in future releases.
py::object _ConstVoidPtr::to_object() const {
  PyErr_WarnEx(PyExc_DeprecationWarning, "ConstVoidPtr.to_object() is deprecated. Use ConstVoidPtr.value() instead.",
               1);
  return this->value();
}

py::object _ConstVoidPtr::value() const {
  if (context_.type() == typeid(py::object)) {
    return std::any_cast<py::object>(context_);
  }
  return py::str(this->c_str());
}

void init_voidptr(py::module &m) {
  //
  // _ConstVoidPtr
  //
  py::class_<_ConstVoidPtr> cvp(m, "ConstVoidPtr", R"doc(
    A wrapper class for the C/C++ ``const void *`` type.
    )doc");

  cvp.def(py::init<const py::object &>(), py::arg("value") = py::none(), R"doc(
    Initialize a ``ConstVoidPtr`` instance with `value`.
    )doc");

  // TODO: Remove deprecated method in future releases.
  cvp.def("to_object", &_ConstVoidPtr::to_object, R"doc(
    .. deprecated:: 0.23
       Use :meth:`.value` instead.
    )doc");

  cvp.def("value", &_ConstVoidPtr::value, R"doc(
    Get the value of the ``ConstVoidPtr`` instance.
    )doc");
}
