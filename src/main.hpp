#ifndef ICUPY_MAIN_HPP
#define ICUPY_MAIN_HPP

#ifdef _MSC_VER
#pragma warning(disable : 4819)

#define strdup _strdup
#endif // _MSC_VER

#include <list>
#include <pybind11/pybind11.h>
// #include <pybind11/stl_bind.h>
#include <unicode/errorcode.h>
#include <unicode/locid.h>
#include <unicode/unistr.h>
#include <variant>
#include <vector>

namespace py = pybind11;

namespace icupy {

using Char16Variant = std::variant<char16_t, uint16_t>;

using LocaleVariant = std::variant<icu::Locale, std::string>;

using UnicodeStringList = std::list<icu::UnicodeString>;
using UnicodeStringVariant = std::variant<icu::UnicodeString, std::u16string>;
using UnicodeStringVector = std::vector<icu::UnicodeString>;

using UChar32Variant = std::variant<char32_t, UChar32>;

inline auto to_char16(const Char16Variant &x) {
  return std::visit([](auto &y) -> char16_t { return y; }, x);
}

inline auto to_locale(const LocaleVariant &x) { return x.index() == 0 ? std::get<0>(x) : std::get<1>(x).c_str(); }

inline auto to_uchar32(const UChar32Variant &x) {
  return std::visit([](auto &y) -> UChar32 { return y; }, x);
}

inline auto to_unistr(const UnicodeStringVariant &x) {
  return x.index() == 0 ? std::get<0>(x) : std::get<1>(x).c_str();
}

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

} // namespace icupy

PYBIND11_MAKE_OPAQUE(icupy::UnicodeStringVector);

#endif // ICUPY_MAIN_HPP
