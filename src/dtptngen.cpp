#include "main.hpp"
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <unicode/dtptngen.h>
#include <unicode/strenum.h>

using namespace icu;

void init_dtptngen(py::module &m) {
  //
  // icu::DateTimePatternGenerator
  //
  py::class_<DateTimePatternGenerator, UObject> dtpg(m, "DateTimePatternGenerator");

  dtpg.def("__copy__", &DateTimePatternGenerator::clone);

  dtpg.def(
      "__deepcopy__", [](const DateTimePatternGenerator &self, py::dict &) { return self.clone(); }, py::arg("memo"));

  dtpg.def(
      "__eq__",
      [](const DateTimePatternGenerator &self, const DateTimePatternGenerator &other) { return self == other; },
      py::is_operator(), py::arg("other"));

  dtpg.def(
      "__ne__",
      [](const DateTimePatternGenerator &self, const DateTimePatternGenerator &other) { return self != other; },
      py::is_operator(), py::arg("other"));

  dtpg.def(
      "add_pattern",
      [](DateTimePatternGenerator &self, const icupy::UnicodeStringVariant &pattern, py::bool_ override,
         UnicodeString &conflicting_pattern) {
        ErrorCode error_code;
        auto result = self.addPattern(icupy::to_unistr(pattern), override, conflicting_pattern, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("pattern"), py::arg("override"), py::arg("conflicting_pattern"));

  dtpg.def("clone", &DateTimePatternGenerator::clone);

  dtpg.def_static("create_empty_instance", []() {
    ErrorCode error_code;
    auto result = DateTimePatternGenerator::createEmptyInstance(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  dtpg.def_static(
          "create_instance",
          [](const icupy::LocaleVariant &locale) {
            ErrorCode error_code;
            auto result = DateTimePatternGenerator::createInstance(icupy::to_locale(locale), error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("locale"))
      .def_static("create_instance", []() {
        ErrorCode error_code;
        auto result = DateTimePatternGenerator::createInstance(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      });

  dtpg.def("get_append_item_format", &DateTimePatternGenerator::getAppendItemFormat, py::arg("field"));

  dtpg.def("get_append_item_name", &DateTimePatternGenerator::getAppendItemName, py::arg("field"));

  dtpg.def(
      "get_base_skeleton",
      [](DateTimePatternGenerator &self, const icupy::UnicodeStringVariant &pattern) {
        ErrorCode error_code;
        auto result = self.getBaseSkeleton(icupy::to_unistr(pattern), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("pattern"));

  dtpg.def("get_base_skeletons", [](const DateTimePatternGenerator &self) {
    ErrorCode error_code;
    auto result = self.getBaseSkeletons(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  dtpg.def(
          "get_best_pattern",
          [](DateTimePatternGenerator &self, const icupy::UnicodeStringVariant &skeleton,
             UDateTimePatternMatchOptions options) {
            ErrorCode error_code;
            auto result = self.getBestPattern(icupy::to_unistr(skeleton), options, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("skeleton"), py::arg("options"))
      .def(
          "get_best_pattern",
          [](DateTimePatternGenerator &self, const icupy::UnicodeStringVariant &skeleton) {
            ErrorCode error_code;
            auto result = self.getBestPattern(icupy::to_unistr(skeleton), error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("skeleton"));

#if (U_ICU_VERSION_MAJOR_NUM >= 71)
  dtpg.def("get_date_time_format", py::overload_cast<>(&DateTimePatternGenerator::getDateTimeFormat, py::const_))
      .def(
          "get_date_time_format",
          [](const DateTimePatternGenerator &self, UDateFormatStyle style) -> const UnicodeString & {
            ErrorCode error_code;
            auto &result = self.getDateTimeFormat(style, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("style"));
#else  // U_ICU_VERSION_MAJOR_NUM <= 70
  dtpg.def("get_date_time_format", &DateTimePatternGenerator::getDateTimeFormat);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 71)

  dtpg.def("get_decimal", &DateTimePatternGenerator::getDecimal);

#if (U_ICU_VERSION_MAJOR_NUM >= 67)
  dtpg.def("get_default_hour_cycle", [](const DateTimePatternGenerator &self) {
    ErrorCode error_code;
    auto result = self.getDefaultHourCycle(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 67)

#if (U_ICU_VERSION_MAJOR_NUM >= 61)
  dtpg.def("get_field_display_name", &DateTimePatternGenerator::getFieldDisplayName, py::arg("field"),
           py::arg("width"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 61)

  dtpg.def(
      "get_pattern_for_skeleton",
      [](DateTimePatternGenerator &self, const icupy::UnicodeStringVariant &skeleton) -> const UnicodeString & {
        return self.getPatternForSkeleton(icupy::to_unistr(skeleton));
      },
      py::arg("skeleton"));

  dtpg.def(
      "get_skeleton",
      [](DateTimePatternGenerator &self, const icupy::UnicodeStringVariant &pattern) {
        ErrorCode error_code;
        auto result = self.getSkeleton(icupy::to_unistr(pattern), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("pattern"));

  dtpg.def("get_skeletons", [](const DateTimePatternGenerator &self) {
    ErrorCode error_code;
    auto result = self.getSkeletons(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  dtpg.def(
          // [1] DateTimePatternGenerator::replaceFieldTypes
          "replace_field_types",
          [](DateTimePatternGenerator &self, const icupy::UnicodeStringVariant &pattern,
             const icupy::UnicodeStringVariant &skeleton, UDateTimePatternMatchOptions options) {
            ErrorCode error_code;
            auto result =
                self.replaceFieldTypes(icupy::to_unistr(pattern), icupy::to_unistr(skeleton), options, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("pattern"), py::arg("skeleton"), py::arg("options"))
      .def(
          // [2] DateTimePatternGenerator::replaceFieldTypes
          "replace_field_types",
          [](DateTimePatternGenerator &self, const icupy::UnicodeStringVariant &pattern,
             const icupy::UnicodeStringVariant &skeleton) {
            ErrorCode error_code;
            auto result = self.replaceFieldTypes(icupy::to_unistr(pattern), icupy::to_unistr(skeleton), error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("pattern"), py::arg("skeleton"));

  dtpg.def(
      "set_append_item_format",
      [](DateTimePatternGenerator &self, UDateTimePatternField field, const icupy::UnicodeStringVariant &value) {
        self.setAppendItemFormat(field, icupy::to_unistr(value));
      },
      py::arg("field"), py::arg("value"));

  dtpg.def(
      "set_append_item_name",
      [](DateTimePatternGenerator &self, UDateTimePatternField field, const icupy::UnicodeStringVariant &value) {
        self.setAppendItemName(field, icupy::to_unistr(value));
      },
      py::arg("field"), py::arg("value"));

  dtpg.def(
          "set_date_time_format",
          [](DateTimePatternGenerator &self, const icupy::UnicodeStringVariant &date_time_format) {
            self.setDateTimeFormat(icupy::to_unistr(date_time_format));
          },
          py::arg("date_time_format"))
#if (U_ICU_VERSION_MAJOR_NUM >= 71)
      .def(
          "set_date_time_format",
          [](DateTimePatternGenerator &self, UDateFormatStyle style,
             const icupy::UnicodeStringVariant &date_time_format) {
            ErrorCode error_code;
            self.setDateTimeFormat(style, icupy::to_unistr(date_time_format), error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
          },
          py::arg("style"), py::arg("date_time_format"))
#endif // (U_ICU_VERSION_MAJOR_NUM >= 71)
      ;

  dtpg.def(
      "set_decimal",
      [](DateTimePatternGenerator &self, const icupy::UnicodeStringVariant &decimal) {
        self.setDecimal(icupy::to_unistr(decimal));
      },
      py::arg("decimal"));

#if (U_ICU_VERSION_MAJOR_NUM >= 56)
  dtpg.def_static(
      "static_get_base_skeleton",
      [](const icupy::UnicodeStringVariant &pattern) {
        ErrorCode error_code;
        auto result = DateTimePatternGenerator::staticGetBaseSkeleton(icupy::to_unistr(pattern), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("pattern"));

  dtpg.def_static(
      "static_get_skeleton",
      [](const icupy::UnicodeStringVariant &pattern) {
        ErrorCode error_code;
        auto result = DateTimePatternGenerator::staticGetSkeleton(icupy::to_unistr(pattern), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("pattern"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 56)
}
