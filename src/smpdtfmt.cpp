#include "main.hpp"
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <unicode/dtfmtsym.h>
#include <unicode/smpdtfmt.h>

using namespace icu;

void init_smpdtfmt(py::module &m) {
  //
  // icu::SimpleDateFormat
  //
  py::class_<SimpleDateFormat, DateFormat> sdf(m, "SimpleDateFormat");

  sdf.def(
         // [1] SimpleDateFormat::SimpleDateFormat
         py::init([]() {
           ErrorCode error_code;
           auto result = std::make_unique<SimpleDateFormat>(error_code);
           if (error_code.isFailure()) {
             throw icupy::ICUError(error_code);
           }
           return result;
         }))
      .def(
          // [2] SimpleDateFormat::SimpleDateFormat
          py::init([](const icupy::UnicodeStringVariant &pattern) {
            ErrorCode error_code;
            auto result = std::make_unique<SimpleDateFormat>(icupy::to_unistr(pattern), error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          }),
          py::arg("pattern"))
      .def(
          // [3] SimpleDateFormat::SimpleDateFormat
          py::init([](const icupy::UnicodeStringVariant &pattern, const icupy::UnicodeStringVariant &override) {
            ErrorCode error_code;
            auto result =
                std::make_unique<SimpleDateFormat>(icupy::to_unistr(pattern), icupy::to_unistr(override), error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          }),
          py::arg("pattern"), py::arg("override"))
      .def(
          // [4] SimpleDateFormat::SimpleDateFormat
          py::init([](const icupy::UnicodeStringVariant &pattern, const Locale &locale) {
            ErrorCode error_code;
            auto result = std::make_unique<SimpleDateFormat>(icupy::to_unistr(pattern), locale, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          }),
          py::arg("pattern"), py::arg("locale"))
      .def(
          // [5] SimpleDateFormat::SimpleDateFormat
          py::init([](const icupy::UnicodeStringVariant &pattern, const icupy::UnicodeStringVariant &override,
                      const Locale &locale) {
            ErrorCode error_code;
            auto result = std::make_unique<SimpleDateFormat>(icupy::to_unistr(pattern), icupy::to_unistr(override),
                                                             locale, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          }),
          py::arg("pattern"), py::arg("override"), py::arg("locale"))
      .def(
          // [7] SimpleDateFormat::SimpleDateFormat
          py::init([](const icupy::UnicodeStringVariant &pattern, const DateFormatSymbols &format_data) {
            ErrorCode error_code;
            auto result = std::make_unique<SimpleDateFormat>(icupy::to_unistr(pattern), format_data, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          }),
          py::arg("pattern"), py::arg("format_data"))
      .def(
          // [8] SimpleDateFormat::SimpleDateFormat
          py::init<const SimpleDateFormat &>(), py::arg("other"));

  sdf.def("__copy__", &SimpleDateFormat::clone);

  sdf.def(
      "__deepcopy__", [](const SimpleDateFormat &self, py::dict &) { return self.clone(); }, py::arg("memo"));

  // FIXME: Implement "void icu::SimpleDateFormat::adoptCalendar(Calendar *calendarToAdopt)".
  // FIXME: Implement "void icu::SimpleDateFormat::adoptDateFormatSymbols(DateFormatSymbols *newFormatSymbols)".
  // FIXME: Implement "void icu::SimpleDateFormat::adoptNumberFormat(const UnicodeString &fields,
  //  NumberFormat *formatToAdopt, UErrorCode &status)".
  // FIXME: Implement "void icu::SimpleDateFormat::adoptNumberFormat(NumberFormat *formatToAdopt)".

  sdf.def(
      "apply_localized_pattern",
      [](SimpleDateFormat &self, const icupy::UnicodeStringVariant &pattern) {
        ErrorCode error_code;
        self.applyLocalizedPattern(icupy::to_unistr(pattern), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("pattern"));

  sdf.def(
      "apply_pattern",
      [](SimpleDateFormat &self, const icupy::UnicodeStringVariant &pattern) {
        self.applyPattern(icupy::to_unistr(pattern));
      },
      py::arg("pattern"));

  sdf.def("clone", &SimpleDateFormat::clone);

  sdf.def("get_2digit_year_start", [](const SimpleDateFormat &self) {
    ErrorCode error_code;
    auto result = self.get2DigitYearStart(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  sdf.def("get_date_format_symbols", &SimpleDateFormat::getDateFormatSymbols, py::return_value_policy::reference);

  // FIXME: Implement "const NumberFormat *icu::SimpleDateFormat::getNumberFormatForField(char16_t field)".

#if (U_ICU_VERSION_MAJOR_NUM >= 50)
  sdf.def("get_time_zone_format", &SimpleDateFormat::getTimeZoneFormat, py::return_value_policy::reference);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 50)

  sdf.def(
      "set_2digit_year_start",
      [](SimpleDateFormat &self, UDate d) {
        ErrorCode error_code;
        self.set2DigitYearStart(d, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("d"));

  sdf.def("set_date_format_symbols", &SimpleDateFormat::setDateFormatSymbols, py::arg("new_format_symbols"));

#if (U_ICU_VERSION_MAJOR_NUM >= 50)
  sdf.def("set_time_zone_format", &SimpleDateFormat::setTimeZoneFormat, py::arg("new_time_zone_format"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 50)

  sdf.def(
      "to_localized_pattern",
      [](const SimpleDateFormat &self, UnicodeString &result) -> UnicodeString & {
        ErrorCode error_code;
        auto &string = self.toLocalizedPattern(result, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return string;
      },
      py::arg("result"));

  sdf.def("to_pattern", &SimpleDateFormat::toPattern, py::arg("result"));
}
