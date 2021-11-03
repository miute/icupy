#include "main.hpp"
#include "uenumptr.hpp"
#include <pybind11/stl.h>
#include <unicode/strenum.h>
#include <unicode/ustring.h>

using namespace icu;

_UEnumerationPtr::_UEnumerationPtr(UEnumeration *p) : p_(p) {}
_UEnumerationPtr::_UEnumerationPtr(UEnumeration *p, const std::shared_ptr<void> &source) : p_(p), source_(source) {}
_UEnumerationPtr::~_UEnumerationPtr() {}
UEnumeration *_UEnumerationPtr::get() const { return p_; }

void init_uenum(py::module &m) {
  py::class_<_UEnumerationPtr>(m, "_UEnumerationPtr");

  m.def(
      "uenum_close", [](_UEnumerationPtr &en) { uenum_close(en); }, py::arg("en"));
  m.def(
      "uenum_count",
      [](_UEnumerationPtr &en) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = uenum_count(en, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("en"));
  m.def(
      "uenum_next",
      [](_UEnumerationPtr &en) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = uenum_next(en, nullptr, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::return_value_policy::reference, py::arg("en"));
#if (U_ICU_VERSION_MAJOR_NUM >= 50)
  m.def(
      "uenum_open_char_strings_enumeration",
      [](const std::vector<std::string> &strings, int32_t count) {
        if (count == -1) {
          count = static_cast<int32_t>(strings.size());
        }
        UErrorCode error_code = U_ZERO_ERROR;
        auto source = std::shared_ptr<char *[]>(new char *[strings.size()], std::default_delete<char *[]>());
        auto s = source.get();
        for (size_t n = 0; n < strings.size(); ++n, ++s) {
          *s = strdup(strings[n].c_str());
        }
        auto p = uenum_openCharStringsEnumeration(source.get(), count, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return std::make_unique<_UEnumerationPtr>(p, source);
      },
      py::arg("strings"), py::arg("count") = -1);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 50)
  m.def(
      "uenum_open_from_string_enumeration",
      [](StringEnumeration *adopted) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto p = uenum_openFromStringEnumeration(adopted ? adopted->clone() : nullptr, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return std::make_unique<_UEnumerationPtr>(p);
      },
      py::arg("adopted"));
#if (U_ICU_VERSION_MAJOR_NUM >= 50)
  m.def(
      "uenum_open_uchar_strings_enumeration",
      [](const std::vector<std::u16string> &strings, int32_t count) {
        if (count == -1) {
          count = static_cast<int32_t>(strings.size());
        }
        UErrorCode error_code = U_ZERO_ERROR;
        auto source = std::shared_ptr<UChar *[]>(new UChar *[strings.size()], std::default_delete<UChar *[]>());
        auto s = source.get();
        for (size_t n = 0; n < strings.size(); ++n, ++s) {
          *s = new UChar[strings[n].size() + 1];
          u_strcpy(*s, strings[n].c_str());
        }
        auto p = uenum_openUCharStringsEnumeration(source.get(), count, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return std::make_unique<_UEnumerationPtr>(p, source);
      },
      py::arg("strings"), py::arg("count") = -1);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 50)
  m.def(
      "uenum_reset",
      [](_UEnumerationPtr &en) {
        UErrorCode error_code = U_ZERO_ERROR;
        uenum_reset(en, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
      },
      py::arg("en"));
  m.def(
      "uenum_unext",
      [](_UEnumerationPtr &en) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = uenum_unext(en, nullptr, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::return_value_policy::reference, py::arg("en"));
}
