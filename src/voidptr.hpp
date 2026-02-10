#ifndef ICUPY_VOIDPTR_HPP
#define ICUPY_VOIDPTR_HPP

#include "main.hpp"
#include <any>

namespace icupy {

class ConstVoidPtr {
public:
  ConstVoidPtr() {};

  ConstVoidPtr(const py::object &value);

  ConstVoidPtr(const void *value);

  ~ConstVoidPtr() {};

  const void *data() const;

  bool has_value() const { return context_.has_value(); }

  py::object to_object() const;

  py::object value() const;

private:
  std::any context_;
};

} // namespace icupy

#endif // ICUPY_VOIDPTR_HPP
