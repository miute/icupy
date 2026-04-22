#ifndef ICUPY_UREGEX_HPP
#define ICUPY_UREGEX_HPP

#include "main.hpp"
#include <functional>
#include <unicode/uregex.h>

namespace icupy {

class UserContext;

using FindProgressCallbackArgs = bool(py::object &, int64_t);

using FindProgressCallbackFunction = std::function<FindProgressCallbackArgs>;

using FindProgressCallbackAndContextPair =
    std::pair<const FindProgressCallbackFunction, const UserContext *>;

using SharedFindProgressCallbackAndContextPair =
    std::shared_ptr<FindProgressCallbackAndContextPair>;

using MatchCallbackArgs = bool(py::object &, int32_t);

using MatchCallbackFunction = std::function<MatchCallbackArgs>;

using MatchCallbackAndContextPair =
    std::pair<const MatchCallbackFunction, const UserContext *>;

using SharedMatchCallbackAndContextPair =
    std::shared_ptr<MatchCallbackAndContextPair>;

//
// URegexFindProgressCallback
//
class URegexFindProgressCallbackPtr {
public:
  URegexFindProgressCallbackPtr() {};

  URegexFindProgressCallbackPtr(
      const std::function<FindProgressCallbackArgs> &action,
      const UserContext *context) {
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

//
// URegexMatchCallback
//
class URegexMatchCallbackPtr {
public:
  URegexMatchCallbackPtr() {};

  URegexMatchCallbackPtr(const std::function<MatchCallbackArgs> &action,
                         const UserContext *context) {
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
