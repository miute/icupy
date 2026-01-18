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
class URegexMatchCallbackPtr;

using FindProgressCallbackArgs = bool(py::object, int);

using FindProgressCallbackAndContextPair =
    std::pair<URegexFindProgressCallbackPtr *, ConstVoidPtr *>;

using SharedFindProgressCallbackAndContextPair =
    std::shared_ptr<FindProgressCallbackAndContextPair>;

using MatchCallbackArgs = bool(py::object &, int32_t);

using MatchCallbackAndContextPair =
    std::pair<const std::function<MatchCallbackArgs>, const ConstVoidPtr *>;

using SharedMatchCallbackAndContextPair =
    std::shared_ptr<MatchCallbackAndContextPair>;

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

class URegexMatchCallbackPtr {
public:
  URegexMatchCallbackPtr() {};

  URegexMatchCallbackPtr(const std::function<MatchCallbackArgs> &action,
                         const ConstVoidPtr *context) {
    action_and_context_ =
        std::make_shared<MatchCallbackAndContextPair>(action, context);
  };

  URegexMatchCallbackPtr(const MatchCallbackAndContextPair *pair) {
    action_and_context_ = std::make_shared<MatchCallbackAndContextPair>(
        pair->first, pair->second);
  };

  ~URegexMatchCallbackPtr() {};

  MatchCallbackAndContextPair *context() const {
    return action_and_context_.get();
  };

  bool empty() const noexcept { return action_and_context_.get() == nullptr; };

  URegexMatchCallback *get_native_callback() {
    return action_and_context_ ? callback : nullptr;
  }

private:
  static UBool callback(const void *context, int32_t steps);

  SharedMatchCallbackAndContextPair action_and_context_;
};

} // namespace icupy

#endif // ICUPY_UREGEX_HPP
