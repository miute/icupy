#include "main.hpp"
#include <pybind11/operators.h>
#include <unicode/dtptngen.h>
#include <unicode/strenum.h>

using namespace icu;

void init_dtptngen(py::module &m) {
  // icu::DateTimePatternGenerator
  py::class_<DateTimePatternGenerator, UObject> dtpg(m, "DateTimePatternGenerator");
  dtpg.def(py::self != py::self, py::arg("other")).def(py::self == py::self, py::arg("other"));
  dtpg.def("__copy__", &DateTimePatternGenerator::clone)
      .def(
          "__deepcopy__", [](const DateTimePatternGenerator &self, py::dict) { return self.clone(); }, py::arg("memo"));
  dtpg.def(
          "add_pattern",
          [](DateTimePatternGenerator &self, const UnicodeString &pattern, UBool override,
             UnicodeString &conflicting_pattern) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.addPattern(pattern, override, conflicting_pattern, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("pattern"), py::arg("override"), py::arg("conflicting_pattern"))
      .def(
          // const char16_t *pattern -> const UnicodeString &pattern
          "add_pattern",
          [](DateTimePatternGenerator &self, const char16_t *pattern, UBool override,
             UnicodeString &conflicting_pattern) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.addPattern(pattern, override, conflicting_pattern, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("pattern"), py::arg("override"), py::arg("conflicting_pattern"));
  dtpg.def("clone", &DateTimePatternGenerator::clone);
  dtpg.def_static("create_empty_instance", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = DateTimePatternGenerator::createEmptyInstance(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  dtpg.def_static(
          "create_instance",
          [](const Locale &locale) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = DateTimePatternGenerator::createInstance(locale, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("locale"))
      .def_static(
          // const char *locale -> const Locale &locale
          "create_instance",
          [](const char *locale) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = DateTimePatternGenerator::createInstance(locale, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("locale"))
      .def_static("create_instance", []() {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = DateTimePatternGenerator::createInstance(error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      });
  dtpg.def("get_append_item_format", &DateTimePatternGenerator::getAppendItemFormat, py::arg("field"));
  dtpg.def("get_append_item_name", &DateTimePatternGenerator::getAppendItemName, py::arg("field"));
  dtpg.def(
          "get_base_skeleton",
          [](DateTimePatternGenerator &self, const UnicodeString &pattern) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.getBaseSkeleton(pattern, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("pattern"))
      .def(
          // const char16_t *pattern -> const UnicodeString &pattern
          "get_base_skeleton",
          [](DateTimePatternGenerator &self, const char16_t *pattern) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.getBaseSkeleton(pattern, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("pattern"));
  dtpg.def("get_base_skeletons", [](const DateTimePatternGenerator &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.getBaseSkeletons(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  dtpg.def(
          "get_best_pattern",
          [](DateTimePatternGenerator &self, const UnicodeString &skeleton, UDateTimePatternMatchOptions options) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.getBestPattern(skeleton, options, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("skeleton"), py::arg("options"))
      .def(
          // const char16_t *skeleton -> const UnicodeString &skeleton
          "get_best_pattern",
          [](DateTimePatternGenerator &self, const char16_t *skeleton, UDateTimePatternMatchOptions options) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.getBestPattern(skeleton, options, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("skeleton"), py::arg("options"))
      .def(
          "get_best_pattern",
          [](DateTimePatternGenerator &self, const UnicodeString &skeleton) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.getBestPattern(skeleton, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("skeleton"))
      .def(
          // const char16_t *skeleton -> const UnicodeString &skeleton
          "get_best_pattern",
          [](DateTimePatternGenerator &self, const char16_t *skeleton) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.getBestPattern(skeleton, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("skeleton"));
  dtpg.def("get_date_time_format", &DateTimePatternGenerator::getDateTimeFormat);
  dtpg.def("get_decimal", &DateTimePatternGenerator::getDecimal);
#if (U_ICU_VERSION_MAJOR_NUM >= 67)
  dtpg.def("get_default_hour_cycle", [](const DateTimePatternGenerator &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.getDefaultHourCycle(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 67)

#if (U_ICU_VERSION_MAJOR_NUM >= 61)
  dtpg.def("get_field_display_name", &DateTimePatternGenerator::getFieldDisplayName, py::arg("field"),
           py::arg("width"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 61)
  dtpg.def("get_pattern_for_skeleton", &DateTimePatternGenerator::getPatternForSkeleton, py::arg("skeleton"))
      .def(
          // const char16_t *skeleton -> const UnicodeString &skeleton
          "get_pattern_for_skeleton",
          [](DateTimePatternGenerator &self, const char16_t *skeleton) -> const UnicodeString & {
            return self.getPatternForSkeleton(skeleton);
          },
          py::arg("skeleton"));
  dtpg.def(
          "get_skeleton",
          [](DateTimePatternGenerator &self, const UnicodeString &pattern) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.getSkeleton(pattern, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("pattern"))
      .def(
          // const char16_t *pattern -> const UnicodeString &pattern
          "get_skeleton",
          [](DateTimePatternGenerator &self, const char16_t *pattern) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.getSkeleton(pattern, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("pattern"));
  dtpg.def("get_skeletons", [](const DateTimePatternGenerator &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.getSkeletons(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  dtpg.def(
          // [1] DateTimePatternGenerator::replaceFieldTypes
          "replace_field_types",
          [](DateTimePatternGenerator &self, const UnicodeString &pattern, const UnicodeString &skeleton,
             UDateTimePatternMatchOptions options) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.replaceFieldTypes(pattern, skeleton, options, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("pattern"), py::arg("skeleton"), py::arg("options"))
      .def(
          // const char16_t *pattern -> const UnicodeString &pattern
          "replace_field_types",
          [](DateTimePatternGenerator &self, const char16_t *pattern, const UnicodeString &skeleton,
             UDateTimePatternMatchOptions options) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.replaceFieldTypes(pattern, skeleton, options, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("pattern"), py::arg("skeleton"), py::arg("options"))
      .def(
          // const char16_t *skeleton -> const UnicodeString &skeleton
          "replace_field_types",
          [](DateTimePatternGenerator &self, const UnicodeString &pattern, const char16_t *skeleton,
             UDateTimePatternMatchOptions options) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.replaceFieldTypes(pattern, skeleton, options, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("pattern"), py::arg("skeleton"), py::arg("options"))
      .def(
          // const char16_t *pattern -> const UnicodeString &pattern
          // const char16_t *skeleton -> const UnicodeString &skeleton
          "replace_field_types",
          [](DateTimePatternGenerator &self, const char16_t *pattern, const char16_t *skeleton,
             UDateTimePatternMatchOptions options) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.replaceFieldTypes(pattern, skeleton, options, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("pattern"), py::arg("skeleton"), py::arg("options"))
      .def(
          // [2] DateTimePatternGenerator::replaceFieldTypes
          "replace_field_types",
          [](DateTimePatternGenerator &self, const UnicodeString &pattern, const UnicodeString &skeleton) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.replaceFieldTypes(pattern, skeleton, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("pattern"), py::arg("skeleton"))
      .def(
          // const char16_t *pattern -> const UnicodeString &pattern
          "replace_field_types",
          [](DateTimePatternGenerator &self, const char16_t *pattern, const UnicodeString &skeleton) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.replaceFieldTypes(pattern, skeleton, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("pattern"), py::arg("skeleton"))
      .def(
          // const char16_t *skeleton -> const UnicodeString &skeleton
          "replace_field_types",
          [](DateTimePatternGenerator &self, const UnicodeString &pattern, const char16_t *skeleton) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.replaceFieldTypes(pattern, skeleton, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("pattern"), py::arg("skeleton"))
      .def(
          // const char16_t *pattern -> const UnicodeString &pattern
          // const char16_t *skeleton -> const UnicodeString &skeleton
          "replace_field_types",
          [](DateTimePatternGenerator &self, const char16_t *pattern, const char16_t *skeleton) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.replaceFieldTypes(pattern, skeleton, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("pattern"), py::arg("skeleton"));
  dtpg.def("set_append_item_format", &DateTimePatternGenerator::setAppendItemFormat, py::arg("field"), py::arg("value"))
      .def(
          // const char16_t *value -> const UnicodeString &value
          "set_append_item_format",
          [](DateTimePatternGenerator &self, UDateTimePatternField field, const char16_t *value) {
            self.setAppendItemFormat(field, value);
          },
          py::arg("field"), py::arg("value"));
  dtpg.def("set_append_item_name", &DateTimePatternGenerator::setAppendItemName, py::arg("field"), py::arg("value"))
      .def(
          // const char16_t *value -> const UnicodeString &value
          "set_append_item_name",
          [](DateTimePatternGenerator &self, UDateTimePatternField field, const char16_t *value) {
            self.setAppendItemName(field, value);
          },
          py::arg("field"), py::arg("value"));
  dtpg.def("set_date_time_format", &DateTimePatternGenerator::setDateTimeFormat, py::arg("date_time_format"))
      .def(
          // const char16_t *date_time_format -> const UnicodeString &date_time_format
          "set_date_time_format",
          [](DateTimePatternGenerator &self, const char16_t *date_time_format) {
            self.setDateTimeFormat(date_time_format);
          },
          py::arg("date_time_format"));
  dtpg.def("set_decimal", &DateTimePatternGenerator::setDecimal, py::arg("decimal"))
      .def(
          // const char16_t *decimal -> const UnicodeString &decimal
          "set_decimal", [](DateTimePatternGenerator &self, const char16_t *decimal) { self.setDecimal(decimal); },
          py::arg("decimal"));
#if (U_ICU_VERSION_MAJOR_NUM >= 56)
  dtpg.def_static(
          "static_get_base_skeleton",
          [](const UnicodeString &pattern) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = DateTimePatternGenerator::staticGetBaseSkeleton(pattern, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("pattern"))
      .def_static(
          // const char16_t *pattern -> const UnicodeString &pattern
          "static_get_base_skeleton",
          [](const char16_t *pattern) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = DateTimePatternGenerator::staticGetBaseSkeleton(pattern, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("pattern"));
  dtpg.def_static(
          "static_get_skeleton",
          [](const UnicodeString &pattern) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = DateTimePatternGenerator::staticGetSkeleton(pattern, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("pattern"))
      .def_static(
          // const char16_t *pattern -> const UnicodeString &pattern
          "static_get_skeleton",
          [](const char16_t *pattern) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = DateTimePatternGenerator::staticGetSkeleton(pattern, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("pattern"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 56)
}
