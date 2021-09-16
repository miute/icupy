#include "main.hpp"
#include <pybind11/operators.h>
#include <unicode/dtitvinf.h>

using namespace icu;

void init_dtitvinf(py::module &m) {
  // icu::DateIntervalInfo
  py::class_<DateIntervalInfo> dii(m, "DateIntervalInfo");
  dii.def(py::init([](const Locale &locale) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<DateIntervalInfo>(locale, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("locale"))
      .def(
          // const char *locale -> const Locale &locale
          py::init([](const char *locale) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<DateIntervalInfo>(locale, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("locale"))
      .def(py::init<const DateIntervalInfo &>(), py::arg("other"))
      .def(py::self != py::self, py::arg("other"))
      .def(py::self == py::self, py::arg("other"));
  dii.def("__copy__", &DateIntervalInfo::clone)
      .def(
          "__deepcopy__", [](const DateIntervalInfo &self, py::dict) { return self.clone(); }, py::arg("memo"));
  dii.def("clone", &DateIntervalInfo::clone);
  dii.def("get_default_order", &DateIntervalInfo::getDefaultOrder);
  dii.def("get_fallback_interval_pattern", &DateIntervalInfo::getFallbackIntervalPattern, py::arg("result"));
  dii.def(
         "get_interval_pattern",
         [](const DateIntervalInfo &self, const UnicodeString &skeleton, UCalendarDateFields field,
            UnicodeString &result) -> UnicodeString & {
           UErrorCode error_code = U_ZERO_ERROR;
           auto &string = self.getIntervalPattern(skeleton, field, result, error_code);
           if (U_FAILURE(error_code)) {
             throw ICUError(error_code);
           }
           return string;
         },
         py::arg("skeleton"), py::arg("field"), py::arg("result"))
      .def(
          // const char16_t *skeleton -> const UnicodeString &skeleton
          "get_interval_pattern",
          [](const DateIntervalInfo &self, const char16_t *skeleton, UCalendarDateFields field,
             UnicodeString &result) -> UnicodeString & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &string = self.getIntervalPattern(skeleton, field, result, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return string;
          },
          py::arg("skeleton"), py::arg("field"), py::arg("result"));
  dii.def(
         "set_fallback_interval_pattern",
         [](DateIntervalInfo &self, const UnicodeString &fallback_pattern) {
           UErrorCode error_code = U_ZERO_ERROR;
           self.setFallbackIntervalPattern(fallback_pattern, error_code);
           if (U_FAILURE(error_code)) {
             throw ICUError(error_code);
           }
         },
         py::arg("fallback_pattern"))
      .def(
          // const char16_t *fallback_pattern -> const UnicodeString &fallback_pattern
          "set_fallback_interval_pattern",
          [](DateIntervalInfo &self, const char16_t *fallback_pattern) {
            UErrorCode error_code = U_ZERO_ERROR;
            self.setFallbackIntervalPattern(fallback_pattern, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
          },
          py::arg("fallback_pattern"));
  dii.def(
         "set_interval_pattern",
         [](DateIntervalInfo &self, const UnicodeString &skeleton, UCalendarDateFields lrg_diff_cal_unit,
            const UnicodeString &interval_pattern) {
           UErrorCode error_code = U_ZERO_ERROR;
           self.setIntervalPattern(skeleton, lrg_diff_cal_unit, interval_pattern, error_code);
           if (U_FAILURE(error_code)) {
             throw ICUError(error_code);
           }
         },
         py::arg("skeleton"), py::arg("lrg_diff_cal_unit"), py::arg("interval_pattern"))
      .def(
          // const char16_t *skeleton -> const UnicodeString &skeleton
          "set_interval_pattern",
          [](DateIntervalInfo &self, const char16_t *skeleton, UCalendarDateFields lrg_diff_cal_unit,
             const UnicodeString &interval_pattern) {
            UErrorCode error_code = U_ZERO_ERROR;
            self.setIntervalPattern(skeleton, lrg_diff_cal_unit, interval_pattern, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
          },
          py::arg("skeleton"), py::arg("lrg_diff_cal_unit"), py::arg("interval_pattern"))
      .def(
          // const char16_t *interval_pattern -> const UnicodeString &interval_pattern
          "set_interval_pattern",
          [](DateIntervalInfo &self, const UnicodeString &skeleton, UCalendarDateFields lrg_diff_cal_unit,
             const char16_t *interval_pattern) {
            UErrorCode error_code = U_ZERO_ERROR;
            self.setIntervalPattern(skeleton, lrg_diff_cal_unit, interval_pattern, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
          },
          py::arg("skeleton"), py::arg("lrg_diff_cal_unit"), py::arg("interval_pattern"))
      .def(
          // const char16_t *skeleton -> const UnicodeString &skeleton
          // const char16_t *interval_pattern -> const UnicodeString &interval_pattern
          "set_interval_pattern",
          [](DateIntervalInfo &self, const char16_t *skeleton, UCalendarDateFields lrg_diff_cal_unit,
             const char16_t *interval_pattern) {
            UErrorCode error_code = U_ZERO_ERROR;
            self.setIntervalPattern(skeleton, lrg_diff_cal_unit, interval_pattern, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
          },
          py::arg("skeleton"), py::arg("lrg_diff_cal_unit"), py::arg("interval_pattern"));
}
