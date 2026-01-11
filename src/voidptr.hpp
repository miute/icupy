#ifndef ICUPY_VOIDPTR_HPP
#define ICUPY_VOIDPTR_HPP

#include "main.hpp"
#include <any>
#include <optional>
#include <pybind11/functional.h>

namespace icupy {

class ConstVoidPtr {
public:
  ConstVoidPtr() {};

  ConstVoidPtr(const py::object &value);

  ConstVoidPtr(const void *value);

  ~ConstVoidPtr() {};

  const py::function &action() const { return action_.value(); }

  const void *data() const;

  bool has_action() const { return action_.has_value(); }

  bool has_value() const { return context_.has_value(); }

  void set_action(const std::optional<py::function> &action) {
    action_ = action;
  }

  py::object to_object() const;

  py::object value() const;

private:
  std::any context_;
  std::optional<py::function> action_;
};

} // namespace icupy

#endif // ICUPY_VOIDPTR_HPP
