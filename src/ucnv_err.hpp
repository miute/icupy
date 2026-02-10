#ifndef ICUPY_UCNV_ERR_HPP
#define ICUPY_UCNV_ERR_HPP

#include "main.hpp"
#include "voidptr.hpp"
#include <functional>
#include <unicode/ucnv.h>

// From icu/source/common/ucnv_bld.h
#ifndef UCNV_ERROR_BUFFER_LENGTH
#define UCNV_ERROR_BUFFER_LENGTH 32
#endif // UCNV_ERROR_BUFFER_LENGTH

namespace icupy {

// UConverterFromUCallback
using FromUCallbackArgs = void(py::object &, UConverterFromUnicodeArgs *,
                               const py::str &, int32_t, UChar32,
                               UConverterCallbackReason, icu::ErrorCode *);

using FromUCallbackFunction = std::function<FromUCallbackArgs>;

using FromUCallbackAndContextPair = std::pair<
    const std::variant<FromUCallbackFunction, UConverterFromUCallback>,
    const void *>;

using SharedFromUCallbackAndContextPair =
    std::shared_ptr<FromUCallbackAndContextPair>;

// UConverterToUCallback
using ToUCallbackArgs = void(py::object &, UConverterToUnicodeArgs *,
                             const py::bytes &, int32_t,
                             UConverterCallbackReason, icu::ErrorCode *);

using ToUCallbackFunction = std::function<ToUCallbackArgs>;

using ToUCallbackAndContextPair =
    std::pair<const std::variant<ToUCallbackFunction, UConverterToUCallback>,
              const void *>;

using SharedToUCallbackAndContextPair =
    std::shared_ptr<ToUCallbackAndContextPair>;

//
// struct UConverter
//
class UConverterPtr {
public:
  UConverterPtr(UConverter *p);

  ~UConverterPtr();

  UConverter *get() const;

  operator UConverter *() const { return get(); }

private:
  UConverterPtr() = delete;
  UConverter *p_;
};

//
// UConverterFromUCallback
//
class UConverterFromUCallbackPtr {
public:
  UConverterFromUCallbackPtr(const FromUCallbackFunction &action,
                             const ConstVoidPtr *context) {
    action_and_context_ =
        std::make_shared<FromUCallbackAndContextPair>(action, context);
  }

  UConverterFromUCallbackPtr(UConverterFromUCallback action,
                             const ConstVoidPtr *context) {
    action_and_context_ = std::make_shared<FromUCallbackAndContextPair>(
        action, context ? context->data() : nullptr);
  }

  UConverterFromUCallbackPtr(UConverterFromUCallback action,
                             const void *context) {
    action_and_context_ =
        std::make_shared<FromUCallbackAndContextPair>(action, context);
  }

  UConverterFromUCallbackPtr(const FromUCallbackAndContextPair *pair) {
    action_and_context_ = std::make_shared<FromUCallbackAndContextPair>(
        pair->first, pair->second);
  }

  ~UConverterFromUCallbackPtr() {}

  FromUCallbackAndContextPair *context() const {
    return action_and_context_.get();
  }

  UConverterFromUCallback get_native_callback() const {
    if (!action_and_context_) {
      return nullptr;
    }
    auto &action = action_and_context_->first;
    return std::holds_alternative<UConverterFromUCallback>(action)
               ? std::get<UConverterFromUCallback>(action)
               : callback;
  }

  bool is_cpp_function() const {
    if (!action_and_context_) {
      return false;
    }
    auto &action = action_and_context_->first;
    return std::holds_alternative<FromUCallbackFunction>(action);
  }

  static bool is_cpp_function(UConverterFromUCallback action) {
    return action == callback;
  }

  const char *option() const {
    auto pair = context();
    if (pair == nullptr || pair->second == nullptr) {
      return nullptr;
    }
    return is_cpp_function() ? nullptr
                             : reinterpret_cast<const char *>(pair->second);
  }

private:
  UConverterFromUCallbackPtr() = delete;

  static void callback(const void *context, UConverterFromUnicodeArgs *args,
                       const UChar *code_units, int32_t length,
                       UChar32 code_point, UConverterCallbackReason reason,
                       UErrorCode *error_code);

  SharedFromUCallbackAndContextPair action_and_context_;
};

//
// UConverterToUCallback
//
class UConverterToUCallbackPtr {
public:
  UConverterToUCallbackPtr(const ToUCallbackFunction &action,
                           const ConstVoidPtr *context) {
    action_and_context_ =
        std::make_shared<ToUCallbackAndContextPair>(action, context);
  }

  UConverterToUCallbackPtr(UConverterToUCallback action,
                           const ConstVoidPtr *context) {
    action_and_context_ = std::make_shared<ToUCallbackAndContextPair>(
        action, context ? context->data() : nullptr);
  }

  UConverterToUCallbackPtr(UConverterToUCallback action, const void *context) {
    action_and_context_ =
        std::make_shared<ToUCallbackAndContextPair>(action, context);
  }

  UConverterToUCallbackPtr(const ToUCallbackAndContextPair *pair) {
    action_and_context_ =
        std::make_shared<ToUCallbackAndContextPair>(pair->first, pair->second);
  }

  ~UConverterToUCallbackPtr() {}

  ToUCallbackAndContextPair *context() const {
    return action_and_context_.get();
  }

  UConverterToUCallback get_native_callback() const {
    if (!action_and_context_) {
      return nullptr;
    }
    auto &action = action_and_context_->first;
    return std::holds_alternative<UConverterToUCallback>(action)
               ? std::get<UConverterToUCallback>(action)
               : callback;
  }

  bool is_cpp_function() const {
    if (!action_and_context_) {
      return false;
    }
    auto &action = action_and_context_->first;
    return std::holds_alternative<ToUCallbackFunction>(action);
  }

  static bool is_cpp_function(UConverterToUCallback action) {
    return action == callback;
  }

  const char *option() const {
    auto pair = context();
    if (pair == nullptr || pair->second == nullptr) {
      return nullptr;
    }
    return is_cpp_function() ? nullptr
                             : reinterpret_cast<const char *>(pair->second);
  }

private:
  UConverterToUCallbackPtr() = delete;

  static void callback(const void *context, UConverterToUnicodeArgs *args,
                       const char *code_units, int32_t length,
                       UConverterCallbackReason reason, UErrorCode *error_code);

  SharedToUCallbackAndContextPair action_and_context_;
};

} // namespace icupy

#endif // ICUPY_UCNV_ERR_HPP
