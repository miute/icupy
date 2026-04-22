#ifndef ICUPY_CONTEXT_HPP
#define ICUPY_CONTEXT_HPP

#include "main.hpp"
#include <any>

namespace icupy {

//
// icupy::UserContext
//
class UserContext {
public:
  UserContext() = default;

  UserContext(const py::object &value);

  UserContext(const void *value);

  ~UserContext() = default;

  const void *data() const;

  bool has_value() const { return context_.has_value(); }

  py::object value() const;

private:
  std::any context_;
};

} // namespace icupy

#endif // ICUPY_CONTEXT_HPP
