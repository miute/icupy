#include "voidptr.hpp"
#include <memory>
#include <pybind11/warnings.h>

using SharedString = std::shared_ptr<std::string>;

namespace icupy {

ConstVoidPtr::ConstVoidPtr(const py::object &value) {
  if (py::isinstance<py::str>(value)) {
    context_ = std::make_shared<std::string>(value.cast<std::string>());
  } else {
    context_ = value;
  }
}

ConstVoidPtr::ConstVoidPtr(const void *value) {
  if (value) {
    context_ = std::make_shared<std::string>(static_cast<const char *>(value));
  }
}

const void *ConstVoidPtr::data() const {
  if (context_.has_value() && context_.type() == typeid(SharedString)) {
    auto p = std::any_cast<SharedString>(context_);
    return p->data();
  }
  return nullptr;
}

// TODO: Remove deprecated method in future releases.
py::object ConstVoidPtr::to_object() const {
  py::warnings::warn("ConstVoidPtr.to_object() is deprecated. "
                     "Use ConstVoidPtr.value() instead.",
                     PyExc_DeprecationWarning, 1);
  return this->value();
}

py::object ConstVoidPtr::value() const {
  if (context_.has_value()) {
    if (context_.type() == typeid(py::object)) {
      return std::any_cast<py::object>(context_);
    } else if (context_.type() == typeid(SharedString)) {
      auto p = std::any_cast<SharedString>(context_);
      return py::str(*p);
    }
  }
  return py::none();
}

} // namespace icupy

void init_voidptr(py::module &m) {
  //
  // icupy::ConstVoidPtr
  //
  py::class_<icupy::ConstVoidPtr> cvp(m, "ConstVoidPtr", R"doc(
    A wrapper class for the C/C++ ``const void *`` type.
    )doc");

  cvp.def(py::init<const py::object &>(), py::arg("value") = py::none(), R"doc(
    Initialize a ``ConstVoidPtr`` instance with `value`.
    )doc");

  // TODO: Remove deprecated method in future releases.
  cvp.def("to_object", &icupy::ConstVoidPtr::to_object, R"doc(
    .. deprecated:: 0.23
       Use :meth:`.value` instead.
    )doc");

  cvp.def("value", &icupy::ConstVoidPtr::value, R"doc(
    Get the value of the ``ConstVoidPtr`` instance.
    )doc");
}
