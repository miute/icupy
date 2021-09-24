#ifndef ICUPY_MAIN_HPP
#define ICUPY_MAIN_HPP

#ifdef _MSC_VER
#pragma warning(disable : 4819)

#define strdup _strdup
#endif // _MSC_VER

#include <list>
#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>
#include <unicode/unistr.h>
#include <vector>

namespace py = pybind11;

using _UnicodeStringList = std::list<icu::UnicodeString>;
using _UnicodeStringVector = std::vector<icu::UnicodeString>;

PYBIND11_MAKE_OPAQUE(_UnicodeStringVector);

class ICUError : public std::exception {
public:
  explicit ICUError(UErrorCode error_code, const char *message = "");
  const char *what() const noexcept override { return what_.c_str(); };
  UErrorCode get_error_code() const { return error_code_; };
  const char *get_message() const { return message_.c_str(); };

private:
  UErrorCode error_code_;
  std::string message_;
  std::string what_;
};

#endif // ICUPY_MAIN_HPP
