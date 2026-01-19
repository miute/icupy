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

using FindProgressCallbackArgs = bool(py::object &, int64_t);

using FindProgressCallbackAndContextPair =
    std::pair<const std::function<FindProgressCallbackArgs>,
              const ConstVoidPtr *>;

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
      const std::function<FindProgressCallbackArgs> &action,
      const ConstVoidPtr *context) {
    action_and_context_ =
        std::make_shared<FindProgressCallbackAndContextPair>(action, context);
  };

  URegexFindProgressCallbackPtr(
      const FindProgressCallbackAndContextPair *pair) {
    action_and_context_ = std::make_shared<FindProgressCallbackAndContextPair>(
        pair->first, pair->second);
  };

  ~URegexFindProgressCallbackPtr() {};

  FindProgressCallbackAndContextPair *context() const {
    return action_and_context_.get();
  };

  bool empty() const noexcept { return action_and_context_.get() == nullptr; };

  URegexFindProgressCallback *get_native_callback() {
    return action_and_context_ ? callback : nullptr;
  }

private:
  static UBool callback(const void *context, int64_t match_index);

  SharedFindProgressCallbackAndContextPair action_and_context_;
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
