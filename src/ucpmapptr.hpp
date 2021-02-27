#ifndef ICUPY_UCPMAPPTR_HPP
#define ICUPY_UCPMAPPTR_HPP

#include "main.hpp"
#include <pybind11/functional.h>
#include <unicode/ucpmap.h>
#include <variant>

class _ConstUCPMapPtr {
public:
  _ConstUCPMapPtr(const UCPMap *p);
  ~_ConstUCPMapPtr();

  const UCPMap *get() const;
  operator const UCPMap *() const { return get(); }

private:
  _ConstUCPMapPtr() = delete;
  const UCPMap *p_;
};

class _UCPMapValueFilterPtr {
public:
  _UCPMapValueFilterPtr(std::nullptr_t filter);
  _UCPMapValueFilterPtr(const py::function &filter);
  ~_UCPMapValueFilterPtr();

  static uint32_t filter(const void *context, uint32_t value);

  template <typename T> T get() const { return std::get<T>(action_); };

  bool has_value() const { return !action_.valueless_by_exception() && action_.index() != 0; };

private:
  _UCPMapValueFilterPtr() = delete;
  std::variant<std::nullptr_t, py::function> action_;
};

#endif // ICUPY_UCPMAPPTR_HPP
