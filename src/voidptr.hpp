#ifndef ICUPY_VOIDPTR_HPP
#define ICUPY_VOIDPTR_HPP

#include "main.hpp"
#include <any>
#include <pybind11/functional.h>

class _ConstVoidPtr {
public:
  _ConstVoidPtr(std::nullptr_t value);
  _ConstVoidPtr(const char *value);
  _ConstVoidPtr(py::object value);
  _ConstVoidPtr(const void *value);
  // template <typename T> _ConstVoidPtr(T value);
  ~_ConstVoidPtr();

  std::any get() const { return context_; }

  py::function get_action() const { return action_; }

  bool has_value() const { return context_.has_value(); }

  void set_action(py::function action) { action_ = action; }

  const char *to_c_str() const;

  py::object to_object() const;

  template <typename T> auto value() const { return std::any_cast<T>(context_); }

private:
  _ConstVoidPtr() = delete;
  std::any context_;
  py::function action_;
};

#endif // ICUPY_VOIDPTR_HPP
