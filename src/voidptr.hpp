#ifndef ICUPY_VOIDPTR_HPP
#define ICUPY_VOIDPTR_HPP

#include "main.hpp"
#include <any>
#include <pybind11/functional.h>

class _ConstVoidPtr {
public:
  _ConstVoidPtr(const py::object &value);

  _ConstVoidPtr(const void *value);

  ~_ConstVoidPtr() {};

  py::function &action() { return action_; }

  const char *c_str() const;

  bool has_value() const { return context_.has_value(); }

  void set_action(const py::function &action) { action_ = action; }

  py::object to_object() const;

  py::object value() const;

private:
  _ConstVoidPtr() = delete;
  std::any context_;
  py::function action_;
};

#endif // ICUPY_VOIDPTR_HPP
