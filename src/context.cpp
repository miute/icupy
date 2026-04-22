#include "context.hpp"
#include <memory>
#include <pybind11/warnings.h>

namespace icupy {

using SharedString = std::shared_ptr<std::string>;

//
// icupy::ConstVoidPtr
// Deprecated alias of UserContext for backward compatibility.
// TODO: Remove ConstVoidPtr in future releases.
//
class ConstVoidPtr : public UserContext {
public:
  using UserContext::UserContext;

  ~ConstVoidPtr() = default;

  using UserContext::value;

  py::object to_object() const { return value(); }
};

//
// icupy::UserContext
//
UserContext::UserContext(const py::object &value) {
  if (py::isinstance<py::str>(value)) {
    context_ = std::make_shared<std::string>(value.cast<std::string>());
  } else {
    context_ = value;
  }
}

UserContext::UserContext(const void *value) {
  if (value) {
    context_ = std::make_shared<std::string>(static_cast<const char *>(value));
  }
}

const void *UserContext::data() const {
  if (context_.has_value() && context_.type() == typeid(SharedString)) {
    auto p = std::any_cast<SharedString>(context_);
    return p->data();
  }
  return nullptr;
}

py::object UserContext::value() const {
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

void init_context(py::module &m) {
  //
  // icupy::UserContext
  //
  py::class_<icupy::UserContext> ctx(m, "UserContext", R"doc(
      Storage type that can hold any value.
      )doc");

  ctx.def(py::init<const py::object &>(), py::arg("value") = py::none(), R"doc(
      Initialize a ``UserContext`` instance with the specified *value*.
      )doc");

  ctx.def("value", &icupy::UserContext::value, R"doc(
      Return the value associated with this object.
      )doc");

  //
  // icupy::ConstVoidPtr
  //
  py::class_<icupy::ConstVoidPtr, icupy::UserContext> cvp(m, "ConstVoidPtr",
                                                          R"doc(
      Alias of :class:`UserContext` for backward compatibility.

      .. deprecated:: 0.24
          ``ConstVoidPtr`` is deprecated and will be removed in future
          releases. Use :class:`UserContext` instead.
      )doc");

  cvp.def(py::init<const py::object &>(), py::arg("value") = py::none(), R"doc(
      Initialize a ``ConstVoidPtr`` instance with the specified *value*.
      )doc");

  cvp.def("to_object", &icupy::ConstVoidPtr::to_object, R"doc(
      Return the value associated with this object.

      .. deprecated:: 0.23
          Use :meth:`.value` instead.
      )doc");
}
