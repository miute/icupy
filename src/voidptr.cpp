#include "voidptr.hpp"

_ConstVoidPtr::_ConstVoidPtr(std::nullptr_t) {}
_ConstVoidPtr::_ConstVoidPtr(const char *value) { context_ = std::shared_ptr<char[]>(strdup(value)); }
_ConstVoidPtr::_ConstVoidPtr(const py::object &value) : context_(value) {}
_ConstVoidPtr::_ConstVoidPtr(const void *value) : context_(value) {}
// template <typename T> _ConstVoidPtr::_ConstVoidPtr(T value) : context_(value) {}
_ConstVoidPtr::~_ConstVoidPtr() {}

const char *_ConstVoidPtr::to_c_str() const {
  if (context_.type() != typeid(std::shared_ptr<char[]>)) {
    return nullptr;
  }
  auto p = std::any_cast<std::shared_ptr<char[]>>(context_);
  return &p[0];
}

py::object _ConstVoidPtr::to_object() const {
  auto python_context = py::cast(nullptr);
  if (context_.has_value()) {
    auto p = this->to_c_str();
    if (p) {
      python_context = py::str(p);
    } else if (context_.type() == typeid(py::object)) {
      python_context = std::any_cast<py::object>(context_);
    }
  }
  return python_context;
}

void init_voidptr(py::module &m) {
  py::class_<_ConstVoidPtr> cvp(m, "ConstVoidPtr");
  cvp.def(py::init<std::nullptr_t>(), py::arg("value"))
      .def(py::init<const char *>(), py::arg("value").none(false))
      .def(py::init<py::object>(), py::arg("value"));
  cvp.def("to_object", &_ConstVoidPtr::to_object);
}
