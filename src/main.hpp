#ifndef ICUPY_MAIN_HPP
#define ICUPY_MAIN_HPP

#ifdef _MSC_VER
#pragma warning(disable : 4819)

#define strdup _strdup
#endif // _MSC_VER

#include <pybind11/pybind11.h>
#include <unicode/errorcode.h>
#include <unicode/locid.h>
#include <unicode/unistr.h>
#include <variant>
#include <vector>

namespace py = pybind11;

namespace icupy {

template <class... Ts> struct overload : Ts... {
  using Ts::operator()...;
};
template <class... Ts> overload(Ts...) -> overload<Ts...>;

using Char16Variant = std::variant<char16_t, uint16_t>;

using CharPtrVariant = std::variant<std::string, py::bytes>;

using LocaleVariant = std::variant<icu::Locale, std::string>;

using UnicodeStringVariant = std::variant<icu::UnicodeString, std::u16string>;

using UnicodeStringVector = std::vector<icu::UnicodeString>;

struct CharPtr {
  CharPtrVariant value;

  CharPtr(CharPtrVariant v) : value(std::move(v)) {}

  inline operator const char *() const {
    return std::visit(overload{[](const std::string &text) -> const char * {
                                 return text.data();
                               },
                               [](const py::bytes &text) -> const char * {
                                 return PyBytes_AS_STRING(text.ptr());
                               }},
                      value);
  }

  std::size_t size() const {
    return std::visit(overload{[](const std::string &text) -> std::size_t {
                                 return text.size();
                               },
                               [](const py::bytes &text) -> std::size_t {
                                 return PyBytes_GET_SIZE(text.ptr());
                               }},
                      value);
  }
};

inline char16_t to_char16(const Char16Variant &value) {
  return std::visit(overload{[](const char16_t c) -> char16_t { return c; },
                             [](const uint16_t c) -> char16_t {
                               return static_cast<char16_t>(c);
                             }},
                    value);
}

inline icu::Locale to_locale(const LocaleVariant &value) {
  return std::visit(
      overload{[](const icu::Locale &name) -> icu::Locale { return name; },
               [](const std::string &name) -> icu::Locale {
                 return icu::Locale(name.data());
               }},
      value);
}

inline icu::UnicodeString to_unistr(const UnicodeStringVariant &value) {
  return std::visit(
      overload{[](const icu::UnicodeString &text) -> icu::UnicodeString {
                 return text;
               },
               [](const std::u16string &text) -> icu::UnicodeString {
                 return icu::UnicodeString(text.data(),
                                           static_cast<int32_t>(text.size()));
               }},
      value);
}

class ICUError : public std::exception {
public:
  explicit ICUError(const icu::ErrorCode &error_code, const char *message = "");

  explicit ICUError(UErrorCode error_code);

  const icu::ErrorCode &error_code() const { return error_code_; };

  const char *what() const noexcept override { return message_.data(); };

private:
  icu::ErrorCode error_code_;
  std::string message_;
};

} // namespace icupy

PYBIND11_MAKE_OPAQUE(icupy::UnicodeStringVector);

#endif // ICUPY_MAIN_HPP
