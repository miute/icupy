#ifndef ICUPY_MAIN_HPP
#define ICUPY_MAIN_HPP

#ifdef _MSC_VER
#pragma warning(disable : 4819)
#endif // _MSC_VER

#include <pybind11/pybind11.h>
#include <unicode/utypes.h>

namespace py = pybind11;

class ICUException : public std::exception {
public:
  explicit ICUException(UErrorCode error_code, const char *message = "");
  const char *what() const noexcept override { return what_.c_str(); };
  UErrorCode get_error_code() const { return error_code_; };
  const char *get_message() const { return message_.c_str(); };

private:
  UErrorCode error_code_;
  std::string message_;
  std::string what_;
};

#endif // ICUPY_MAIN_HPP
