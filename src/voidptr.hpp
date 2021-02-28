#ifndef ICUPY_VOIDPTR_HPP
#define ICUPY_VOIDPTR_HPP

#include "main.hpp"
#include <any>
#include <pybind11/functional.h>

class _ConstVoidPtr {
public:
  _ConstVoidPtr(std::nullptr_t value);
  _ConstVoidPtr(const char *value);
  _ConstVoidPtr(const py::object &value);
  _ConstVoidPtr(const void *value);
  // template <typename T> _ConstVoidPtr(T value);
  ~_ConstVoidPtr();

  py::function &get_action() { return action_; }

  bool has_value() const { return context_.has_value(); }

  void set_action(const py::function &action) { action_ = action; }

  const char *to_c_str() const;

  py::object to_object() const;

private:
  _ConstVoidPtr() = delete;
  std::any context_;
  py::function action_;
};

#endif // ICUPY_VOIDPTR_HPP
