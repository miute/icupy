#include "main.hpp"
#include "uenumptr.hpp"
#include <pybind11/stl.h>
#include <unicode/strenum.h>
#include <unicode/ustring.h>

using namespace icu;

namespace icupy {

//
// struct UEnumeration
//
UEnumerationPtr::UEnumerationPtr(UEnumeration *p) : p_(p) {}

UEnumerationPtr::UEnumerationPtr(UEnumeration *p,
                                 const std::shared_ptr<void> &source)
    : p_(p), source_(source) {}

UEnumerationPtr::~UEnumerationPtr() {}

UEnumeration *UEnumerationPtr::get() const { return p_; }

} // namespace icupy

void init_uenum(py::module &m) {
  //
  // struct UEnumeration
  //
  py::class_<icupy::UEnumerationPtr>(m, "UEnumeration", R"doc(
    UEnumeration structure representing an enumeration.

    See Also:
        :func:`uenum_close`
        :func:`uenum_open_char_strings_enumeration`
        :func:`uenum_open_from_string_enumeration`
        :func:`uenum_open_uchar_strings_enumeration`
    )doc");

  //
  // Functions
  //
  m.def(
      "uenum_close", [](icupy::UEnumerationPtr &en) { uenum_close(en); },
      py::arg("en"));

  m.def(
      "uenum_count",
      [](icupy::UEnumerationPtr &en) {
        ErrorCode error_code;
        auto result = uenum_count(en, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("en"));

  m.def(
      "uenum_next",
      [](icupy::UEnumerationPtr &en) {
        ErrorCode error_code;
        auto result = uenum_next(en, nullptr, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
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
        auto strings_ptr = std::shared_ptr<char *[]>();
        if (count > 0) {
          strings_ptr = std::shared_ptr<char *[]>(
              new char *[count], std::default_delete<char *[]>());
          auto pp = strings_ptr.get();
          for (size_t n = 0; n < count; ++n, ++pp) {
            *pp = strdup(strings[n].c_str());
          }
        }
        ErrorCode error_code;
        auto p = uenum_openCharStringsEnumeration(strings_ptr.get(), count,
                                                  error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return std::make_unique<icupy::UEnumerationPtr>(p, strings_ptr);
      },
      py::arg("strings"), py::arg("count") = -1);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 50)

  m.def(
      "uenum_open_from_string_enumeration",
      [](StringEnumeration *adopted) {
        ErrorCode error_code;
        auto p = uenum_openFromStringEnumeration(adopted->clone(), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return std::make_unique<icupy::UEnumerationPtr>(p);
      },
      py::arg("adopted").none(false));

#if (U_ICU_VERSION_MAJOR_NUM >= 50)
  m.def(
      "uenum_open_uchar_strings_enumeration",
      [](const std::vector<std::u16string> &strings, int32_t count) {
        if (count == -1) {
          count = static_cast<int32_t>(strings.size());
        }
        auto strings_ptr = std::shared_ptr<UChar *[]>();
        if (count > 0) {
          strings_ptr = std::shared_ptr<UChar *[]>(
              new UChar *[count], std::default_delete<UChar *[]>());
          auto pp = strings_ptr.get();
          for (size_t n = 0; n < count; ++n, ++pp) {
            *pp = new UChar[strings[n].size() + 1];
            u_strcpy(*pp, strings[n].c_str());
          }
        }
        ErrorCode error_code;
        auto p = uenum_openUCharStringsEnumeration(strings_ptr.get(), count,
                                                   error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return std::make_unique<icupy::UEnumerationPtr>(p, strings_ptr);
      },
      py::arg("strings"), py::arg("count") = -1);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 50)

  m.def(
      "uenum_reset",
      [](icupy::UEnumerationPtr &en) {
        ErrorCode error_code;
        uenum_reset(en, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("en"));

  m.def(
      "uenum_unext",
      [](icupy::UEnumerationPtr &en) {
        ErrorCode error_code;
        auto result = uenum_unext(en, nullptr, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::return_value_policy::reference, py::arg("en"));
}
