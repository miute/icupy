#include "main.hpp"
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <unicode/dtitvinf.h>

using namespace icu;

void init_dtitvinf(py::module &m) {
  //
  // icu::DateIntervalInfo
  //
  py::class_<DateIntervalInfo> dii(m, "DateIntervalInfo");

  dii.def(py::init([](const _LocaleVariant &locale) {
            ErrorCode error_code;
            auto result = std::make_unique<DateIntervalInfo>(VARIANT_TO_LOCALE(locale), error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("locale"))
      .def(py::init<const DateIntervalInfo &>(), py::arg("other"));

  dii.def("__copy__", &DateIntervalInfo::clone);

  dii.def(
      "__deepcopy__", [](const DateIntervalInfo &self, py::dict &) { return self.clone(); }, py::arg("memo"));

  dii.def(
      "__eq__", [](const DateIntervalInfo &self, const DateIntervalInfo &other) { return self == other; },
      py::is_operator(), py::arg("other"));

  dii.def(
      "__ne__", [](const DateIntervalInfo &self, const DateIntervalInfo &other) { return self != other; },
      py::is_operator(), py::arg("other"));

  dii.def("clone", &DateIntervalInfo::clone);

  dii.def("get_default_order", &DateIntervalInfo::getDefaultOrder);

  dii.def("get_fallback_interval_pattern", &DateIntervalInfo::getFallbackIntervalPattern, py::arg("result"));

  dii.def(
      "get_interval_pattern",
      [](const DateIntervalInfo &self, const _UnicodeStringVariant &skeleton, UCalendarDateFields field,
         UnicodeString &result) -> UnicodeString & {
        ErrorCode error_code;
        auto &string = self.getIntervalPattern(VARIANT_TO_UNISTR(skeleton), field, result, error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return string;
      },
      py::arg("skeleton"), py::arg("field"), py::arg("result"));

  dii.def(
      "set_fallback_interval_pattern",
      [](DateIntervalInfo &self, const _UnicodeStringVariant &fallback_pattern) {
        ErrorCode error_code;
        self.setFallbackIntervalPattern(VARIANT_TO_UNISTR(fallback_pattern), error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
      },
      py::arg("fallback_pattern"));

  dii.def(
      "set_interval_pattern",
      [](DateIntervalInfo &self, const _UnicodeStringVariant &skeleton, UCalendarDateFields lrg_diff_cal_unit,
         const _UnicodeStringVariant &interval_pattern) {
        ErrorCode error_code;
        self.setIntervalPattern(VARIANT_TO_UNISTR(skeleton), lrg_diff_cal_unit, VARIANT_TO_UNISTR(interval_pattern),
                                error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
      },
      py::arg("skeleton"), py::arg("lrg_diff_cal_unit"), py::arg("interval_pattern"));
}
