#include "main.hpp"
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <unicode/dtitvinf.h>

using namespace icu;

void init_dtitvinf(py::module &m) {
  //
  // icu::DateIntervalInfo
  //
  py::class_<DateIntervalInfo, UObject> dii(m, "DateIntervalInfo");

  dii.def(py::init([](const icupy::LocaleVariant &locale) {
            ErrorCode error_code;
            auto result = std::make_unique<DateIntervalInfo>(icupy::to_locale(locale), error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
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

  dii.def("get_default_order", [](const DateIntervalInfo &self) -> py::bool_ { return self.getDefaultOrder(); });

  dii.def("get_fallback_interval_pattern", &DateIntervalInfo::getFallbackIntervalPattern, py::arg("result"));

  dii.def(
      "get_interval_pattern",
      [](const DateIntervalInfo &self, const icupy::UnicodeStringVariant &skeleton, UCalendarDateFields field,
         UnicodeString &result) -> UnicodeString & {
        ErrorCode error_code;
        auto &string = self.getIntervalPattern(icupy::to_unistr(skeleton), field, result, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return string;
      },
      py::arg("skeleton"), py::arg("field"), py::arg("result"));

  dii.def(
      "set_fallback_interval_pattern",
      [](DateIntervalInfo &self, const icupy::UnicodeStringVariant &fallback_pattern) {
        ErrorCode error_code;
        self.setFallbackIntervalPattern(icupy::to_unistr(fallback_pattern), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("fallback_pattern"));

  dii.def(
      "set_interval_pattern",
      [](DateIntervalInfo &self, const icupy::UnicodeStringVariant &skeleton, UCalendarDateFields lrg_diff_cal_unit,
         const icupy::UnicodeStringVariant &interval_pattern) {
        ErrorCode error_code;
        self.setIntervalPattern(icupy::to_unistr(skeleton), lrg_diff_cal_unit, icupy::to_unistr(interval_pattern),
                                error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("skeleton"), py::arg("lrg_diff_cal_unit"), py::arg("interval_pattern"));
}
