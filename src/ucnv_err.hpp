#ifndef ICUPY_UCNV_ERR_HPP
#define ICUPY_UCNV_ERR_HPP

#include "main.hpp"
#include <pybind11/functional.h>
#include <unicode/ucnv.h>
#include <variant>

// From icu/source/common/ucnv_bld.h
#ifndef UCNV_ERROR_BUFFER_LENGTH
#define UCNV_ERROR_BUFFER_LENGTH 32
#endif // UCNV_ERROR_BUFFER_LENGTH

class _UConverterPtr {
public:
  _UConverterPtr(UConverter *p);
  ~_UConverterPtr();

  UConverter *get() const;
  operator UConverter *() const { return get(); }

private:
  _UConverterPtr() = delete;
  UConverter *p_;
};

class _UConverterFromUCallbackPtr {
public:
  _UConverterFromUCallbackPtr(UConverterFromUCallback action);
  _UConverterFromUCallbackPtr(const py::function &action);
  ~_UConverterFromUCallbackPtr();

  static void callback(const void *context, UConverterFromUnicodeArgs *args, const UChar *code_units, int32_t length,
                       UChar32 code_point, UConverterCallbackReason reason, UErrorCode *error_code);

  template <typename T> T get() const { return std::get<T>(action_); };

  template <typename T> auto get_if() const {
    return std::holds_alternative<T>(action_) ? std::get<T>(action_) : nullptr;
  };

private:
  _UConverterFromUCallbackPtr() = delete;
  std::variant<UConverterFromUCallback, py::function> action_;
};

class _UConverterToUCallbackPtr {
public:
  _UConverterToUCallbackPtr(UConverterToUCallback action);
  _UConverterToUCallbackPtr(const py::function &action);
  ~_UConverterToUCallbackPtr();

  static void callback(const void *context, UConverterToUnicodeArgs *args, const char *code_units, int32_t length,
                       UConverterCallbackReason reason, UErrorCode *error_code);

  template <typename T> T get() const { return std::get<T>(action_); };

  template <typename T> auto get_if() const {
    return std::holds_alternative<T>(action_) ? std::get<T>(action_) : nullptr;
  };

private:
  _UConverterToUCallbackPtr() = delete;
  std::variant<UConverterToUCallback, py::function> action_;
};

#endif // ICUPY_UCNV_ERR_HPP
