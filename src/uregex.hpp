#ifndef ICUPY_UREGEX_HPP
#define ICUPY_UREGEX_HPP

#include "main.hpp"
#include <pybind11/functional.h>
#include <unicode/uregex.h>
#include <variant>

class _URegexFindProgressCallbackPtr {
public:
  _URegexFindProgressCallbackPtr(URegexFindProgressCallback *action);
  _URegexFindProgressCallbackPtr(const py::function &action);
  ~_URegexFindProgressCallbackPtr();

  static UBool callback(const void *context, int64_t match_index);

  template <typename T> T &get() { return std::get<T>(action_); };

  template <typename T> auto get_if() { return std::holds_alternative<T>(action_) ? std::get<T>(action_) : nullptr; };

  bool has_value() {
    return !action_.valueless_by_exception() && (get_if<URegexFindProgressCallback *>() || action_.index() != 0);
  };

private:
  _URegexFindProgressCallbackPtr() = delete;
  std::variant<URegexFindProgressCallback *, py::function> action_;
};

class _URegexMatchCallbackPtr {
public:
  _URegexMatchCallbackPtr(URegexMatchCallback *action);
  _URegexMatchCallbackPtr(const py::function &action);
  ~_URegexMatchCallbackPtr();

  static UBool callback(const void *context, int32_t steps);

  template <typename T> T &get() { return std::get<T>(action_); };

  template <typename T> auto get_if() { return std::holds_alternative<T>(action_) ? std::get<T>(action_) : nullptr; };

  bool has_value() {
    return !action_.valueless_by_exception() && (get_if<URegexMatchCallback *>() || action_.index() != 0);
  };

private:
  _URegexMatchCallbackPtr() = delete;
  std::variant<URegexMatchCallback *, py::function> action_;
};

#endif // ICUPY_UREGEX_HPP
