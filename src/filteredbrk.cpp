#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM >= 56)
#include <pybind11/stl.h>
#include <unicode/filteredbrk.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 56)

using namespace icu;

void init_filteredbrk(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 56)
  // icu::FilteredBreakIteratorBuilder
  py::class_<FilteredBreakIteratorBuilder, UObject> fbib(m, "FilteredBreakIteratorBuilder");
#ifndef U_FORCE_HIDE_DEPRECATED_API
  fbib.def(
      "build",
      [](FilteredBreakIteratorBuilder &self, BreakIterator *adopt_break_iterator) {
        ErrorCode error_code;
        auto result = self.build(adopt_break_iterator ? adopt_break_iterator->clone() : nullptr, error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
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
      throw ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 60)
  fbib.def_static(
      "create_instance",
      [](const _LocaleVariant &where) {
        ErrorCode error_code;
        auto result = FilteredBreakIteratorBuilder::createInstance(VARIANT_TO_LOCALE(where), error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("where"));
#ifndef U_HIDE_DEPRECATED_API
  fbib.def_static("create_instance", []() {
    ErrorCode error_code;
    auto result = FilteredBreakIteratorBuilder::createInstance(error_code);
    if (error_code.isFailure()) {
      throw ICUError(error_code);
    }
    return result;
  });
#endif // U_HIDE_DEPRECATED_API
  fbib.def(
      "suppress_break_after",
      [](FilteredBreakIteratorBuilder &self, const _UnicodeStringVariant &string) {
        ErrorCode error_code;
        auto result = self.suppressBreakAfter(VARIANT_TO_UNISTR(string), error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("string"));
  fbib.def(
      "unsuppress_break_after",
      [](FilteredBreakIteratorBuilder &self, const _UnicodeStringVariant &string) {
        ErrorCode error_code;
        auto result = self.unsuppressBreakAfter(VARIANT_TO_UNISTR(string), error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
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
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("adopt_break_iterator"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 60)

#endif // (U_ICU_VERSION_MAJOR_NUM >= 56)
}
