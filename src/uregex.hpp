#ifndef ICUPY_UREGEX_HPP
#define ICUPY_UREGEX_HPP

#include "main.hpp"
#include <functional>
#include <optional>
#include <pybind11/functional.h>
#include <unicode/uregex.h>

namespace icupy {

class ConstVoidPtr;
class URegexFindProgressCallbackPtr;

using FindProgressCallbackArgs = bool(py::object, int);

using FindProgressCallbackAndContextPair =
    std::pair<URegexFindProgressCallbackPtr *, ConstVoidPtr *>;

using SharedFindProgressCallbackAndContextPair =
    std::shared_ptr<FindProgressCallbackAndContextPair>;

class URegexFindProgressCallbackPtr {
public:
  URegexFindProgressCallbackPtr() {};

  URegexFindProgressCallbackPtr(
      const std::function<FindProgressCallbackArgs> &action)
      : action_(action) {};

  ~URegexFindProgressCallbackPtr() {};

  const std::function<FindProgressCallbackArgs> &action() const {
    return action_.value();
  };

  URegexFindProgressCallback *get_native_callback() {
    return action_.has_value() ? callback : nullptr;
  }

  bool has_action() const { return action_.has_value(); };

  FindProgressCallbackAndContextPair *set_context(ConstVoidPtr *context) {
    callback_and_context_ =
        std::make_shared<FindProgressCallbackAndContextPair>(
            std::make_pair(this, context));
    return callback_and_context_.get();
  }

private:
  static UBool callback(const void *context, int64_t match_index);

  std::optional<std::function<FindProgressCallbackArgs>> action_;
  SharedFindProgressCallbackAndContextPair callback_and_context_;
};

} // namespace icupy

class _URegexMatchCallbackPtr {
public:
  _URegexMatchCallbackPtr(URegexMatchCallback *action);

  _URegexMatchCallbackPtr(const py::function &action);

  ~_URegexMatchCallbackPtr();

  static UBool callback(const void *context, int32_t steps);

  template <typename T> T &get() { return std::get<T>(action_); };

  template <typename T> auto get_if() {
    return std::holds_alternative<T>(action_) ? std::get<T>(action_) : nullptr;
  };

  bool has_value() {
    return !action_.valueless_by_exception() &&
           (get_if<URegexMatchCallback *>() || action_.index() != 0);
  };

private:
  _URegexMatchCallbackPtr() = delete;
  std::variant<URegexMatchCallback *, py::function> action_;
};

#endif // ICUPY_UREGEX_HPP
