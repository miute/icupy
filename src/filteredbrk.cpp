#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM >= 56)
#include <pybind11/stl.h>
#include <unicode/filteredbrk.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 56)

using namespace icu;

void init_filteredbrk(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 56)
  //
  // icu::FilteredBreakIteratorBuilder
  //
  py::class_<FilteredBreakIteratorBuilder, UObject> fbib(m, "FilteredBreakIteratorBuilder");

#ifndef U_FORCE_HIDE_DEPRECATED_API
  fbib.def(
      "build",
      [](FilteredBreakIteratorBuilder &self, BreakIterator *adopt_break_iterator) {
        ErrorCode error_code;
        auto result = self.build(adopt_break_iterator ? adopt_break_iterator->clone() : nullptr, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("adopt_break_iterator"));
#endif // U_FORCE_HIDE_DEPRECATED_API

#if (U_ICU_VERSION_MAJOR_NUM >= 60)
  fbib.def_static("create_empty_instance", []() {
    ErrorCode error_code;
    auto result = FilteredBreakIteratorBuilder::createEmptyInstance(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 60)

  fbib.def_static(
      "create_instance",
      [](const icupy::LocaleVariant &where) {
        ErrorCode error_code;
        auto result = FilteredBreakIteratorBuilder::createInstance(icupy::to_locale(where), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("where"));

#ifndef U_HIDE_DEPRECATED_API
  fbib.def_static("create_instance", []() {
    ErrorCode error_code;
    auto result = FilteredBreakIteratorBuilder::createInstance(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // U_HIDE_DEPRECATED_API

  fbib.def(
      "suppress_break_after",
      [](FilteredBreakIteratorBuilder &self, const icupy::UnicodeStringVariant &string) -> py::bool_ {
        ErrorCode error_code;
        auto result = self.suppressBreakAfter(icupy::to_unistr(string), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("string"));

  fbib.def(
      "unsuppress_break_after",
      [](FilteredBreakIteratorBuilder &self, const icupy::UnicodeStringVariant &string) -> py::bool_ {
        ErrorCode error_code;
        auto result = self.unsuppressBreakAfter(icupy::to_unistr(string), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("string"));

#if (U_ICU_VERSION_MAJOR_NUM >= 60)
  fbib.def(
      "wrap_iterator_with_filter",
      [](FilteredBreakIteratorBuilder &self, BreakIterator *adopt_break_iterator) {
        ErrorCode error_code;
        auto result =
            self.wrapIteratorWithFilter(adopt_break_iterator ? adopt_break_iterator->clone() : nullptr, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("adopt_break_iterator"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 60)

#endif // (U_ICU_VERSION_MAJOR_NUM >= 56)
}
