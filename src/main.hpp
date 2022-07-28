#ifndef ICUPY_MAIN_HPP
#define ICUPY_MAIN_HPP

#ifdef _MSC_VER
#pragma warning(disable : 4819)

#define strdup _strdup
#endif // _MSC_VER

#include <list>
#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>
#include <unicode/errorcode.h>
#include <unicode/locid.h>
#include <unicode/unistr.h>
#include <variant>
#include <vector>

namespace py = pybind11;

using _Char16Variant = std::variant<char16_t, uint16_t>;

using _LocaleVariant = std::variant<icu::Locale, std::string>;

using _UnicodeStringList = std::list<icu::UnicodeString>;
using _UnicodeStringVariant = std::variant<icu::UnicodeString, std::u16string>;
using _UnicodeStringVector = std::vector<icu::UnicodeString>;

PYBIND11_MAKE_OPAQUE(_UnicodeStringVector);

class ICUError : public std::exception {
public:
  explicit ICUError(const icu::ErrorCode &error_code, const char *message = "");
  explicit ICUError(UErrorCode error_code);
  const icu::ErrorCode &get_error_code() const { return error_code_; };
  const char *get_message() const { return message_.c_str(); };

private:
  icu::ErrorCode error_code_;
  std::string message_;
};

inline auto VARIANT_TO_CHAR16(const _Char16Variant &x) {
  return std::visit([](auto &y) -> char16_t { return y; }, x);
}

inline auto VARIANT_TO_LOCALE(const _LocaleVariant &x) {
  return x.index() == 0 ? std::get<0>(x) : std::get<1>(x).c_str();
}

inline auto VARIANT_TO_UNISTR(const _UnicodeStringVariant &x) {
  return x.index() == 0 ? std::get<0>(x) : std::get<1>(x).c_str();
}

#endif // ICUPY_MAIN_HPP
