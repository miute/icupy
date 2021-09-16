#include "main.hpp"
#include <pybind11/operators.h>
#include <unicode/dtfmtsym.h>
#include <unicode/smpdtfmt.h>

using namespace icu;

void init_smpdtfmt(py::module &m) {
  // icu::SimpleDateFormat
  py::class_<SimpleDateFormat, DateFormat> sdf(m, "SimpleDateFormat");
  sdf.def(
         // [1] SimpleDateFormat::SimpleDateFormat
         py::init([]() {
           UErrorCode error_code = U_ZERO_ERROR;
           auto result = std::make_unique<SimpleDateFormat>(error_code);
           if (U_FAILURE(error_code)) {
             throw ICUError(error_code);
           }
           return result;
         }))
      .def(
          // [2] SimpleDateFormat::SimpleDateFormat
          py::init([](const UnicodeString &pattern) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<SimpleDateFormat>(pattern, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("pattern"))
      .def(
          // const char16_t *pattern -> const UnicodeString &pattern
          py::init([](const char16_t *pattern) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<SimpleDateFormat>(pattern, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("pattern"))
      .def(
          // [3] SimpleDateFormat::SimpleDateFormat
          py::init([](const UnicodeString &pattern, const UnicodeString &override) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<SimpleDateFormat>(pattern, override, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("pattern"), py::arg("override"))
      .def(
          // const char16_t *pattern -> const UnicodeString &pattern
          py::init([](const char16_t *pattern, const UnicodeString &override) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<SimpleDateFormat>(pattern, override, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("pattern"), py::arg("override"))
      .def(
          // const char16_t *override -> const UnicodeString &override
          py::init([](const UnicodeString &pattern, const char16_t *override) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<SimpleDateFormat>(pattern, override, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("pattern"), py::arg("override"))
      .def(
          // const char16_t *pattern -> const UnicodeString &pattern
          // const char16_t *override -> const UnicodeString &override
          py::init([](const char16_t *pattern, const char16_t *override) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<SimpleDateFormat>(pattern, override, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("pattern"), py::arg("override"))
      .def(
          // [4] SimpleDateFormat::SimpleDateFormat
          py::init([](const UnicodeString &pattern, const Locale &locale) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<SimpleDateFormat>(pattern, locale, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("pattern"), py::arg("locale"))
      .def(
          // const char16_t *pattern -> const UnicodeString &pattern
          py::init([](const char16_t *pattern, const Locale &locale) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<SimpleDateFormat>(pattern, locale, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("pattern"), py::arg("locale"))
      .def(
          // [5] SimpleDateFormat::SimpleDateFormat
          py::init([](const UnicodeString &pattern, const UnicodeString &override, const Locale &locale) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<SimpleDateFormat>(pattern, override, locale, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("pattern"), py::arg("override"), py::arg("locale"))
      .def(
          // const char16_t *pattern -> const UnicodeString &pattern
          py::init([](const char16_t *pattern, const UnicodeString &override, const Locale &locale) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<SimpleDateFormat>(pattern, override, locale, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("pattern"), py::arg("override"), py::arg("locale"))
      .def(
          // const char16_t *override -> const UnicodeString &override
          py::init([](const UnicodeString &pattern, const char16_t *override, const Locale &locale) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<SimpleDateFormat>(pattern, override, locale, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("pattern"), py::arg("override"), py::arg("locale"))
      .def(
          // const char16_t *pattern -> const UnicodeString &pattern
          // const char16_t *override -> const UnicodeString &override
          py::init([](const char16_t *pattern, const char16_t *override, const Locale &locale) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<SimpleDateFormat>(pattern, override, locale, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("pattern"), py::arg("override"), py::arg("locale"))
      .def(
          // [7] SimpleDateFormat::SimpleDateFormat
          py::init([](const UnicodeString &pattern, const DateFormatSymbols &format_data) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<SimpleDateFormat>(pattern, format_data, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("pattern"), py::arg("format_data"))
      .def(
          // const char16_t *pattern -> const UnicodeString &pattern
          py::init([](const char16_t *pattern, const DateFormatSymbols &format_data) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<SimpleDateFormat>(pattern, format_data, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("pattern"), py::arg("format_data"))
      .def(
          // [8] SimpleDateFormat::SimpleDateFormat
          py::init<const SimpleDateFormat &>(), py::arg("source"))
      .def(py::self == py::self, py::arg("other"));
  sdf.def("__copy__", &SimpleDateFormat::clone)
      .def(
          "__deepcopy__", [](const SimpleDateFormat &self, py::dict) { return self.clone(); }, py::arg("memo"));
  // FIXME: Implement "void icu::SimpleDateFormat::adoptCalendar(Calendar *calendarToAdopt)".
  // FIXME: Implement "void icu::SimpleDateFormat::adoptDateFormatSymbols(DateFormatSymbols *newFormatSymbols)".
  // FIXME: Implement "void icu::SimpleDateFormat::adoptNumberFormat(const UnicodeString &fields,
  // NumberFormat *formatToAdopt, UErrorCode &status)".
  // FIXME: Implement "void icu::SimpleDateFormat::adoptNumberFormat(NumberFormat *formatToAdopt)".
  sdf.def(
         "apply_localized_pattern",
         [](SimpleDateFormat &self, const UnicodeString &pattern) {
           UErrorCode error_code = U_ZERO_ERROR;
           self.applyLocalizedPattern(pattern, error_code);
           if (U_FAILURE(error_code)) {
             throw ICUError(error_code);
           }
         },
         py::arg("pattern"))
      .def(
          // const char16_t *pattern -> const UnicodeString &pattern
          "apply_localized_pattern",
          [](SimpleDateFormat &self, const char16_t *pattern) {
            UErrorCode error_code = U_ZERO_ERROR;
            self.applyLocalizedPattern(pattern, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
          },
          py::arg("pattern"));
  sdf.def("apply_pattern", &SimpleDateFormat::applyPattern, py::arg("pattern"))
      .def(
          // const char16_t *pattern -> const UnicodeString &pattern
          "apply_pattern", [](SimpleDateFormat &self, const char16_t *pattern) { self.applyPattern(pattern); },
          py::arg("pattern"));
  sdf.def("clone", &SimpleDateFormat::clone);
  sdf.def(
         // [2] SimpleDateFormat::format
         // [1] DateFormat::format
         "format",
         py::overload_cast<Calendar &, UnicodeString &, FieldPosition &>(&SimpleDateFormat::format, py::const_),
         py::arg("cal"), py::arg("append_to"), py::arg("field_position"))
      .def(
          // [4] SimpleDateFormat::format
          // [2] DateFormat::format
          "format",
          [](const SimpleDateFormat &self, Calendar &cal, UnicodeString &append_to,
             FieldPositionIterator *pos_iter) -> UnicodeString & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.format(cal, append_to, pos_iter, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("cal"), py::arg("append_to"), py::arg("pos_iter"))
      .def(
          // [3] DateFormat::format
          // [1] Format::format
          "format",
          [](const SimpleDateFormat &self, const Formattable &obj, UnicodeString &append_to,
             FieldPosition &pos) -> UnicodeString & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.format(obj, append_to, pos, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("obj"), py::arg("append_to"), py::arg("pos"))
      .def(
          // [6] DateFormat::format
          // [2] Format::format
          "format",
          [](const SimpleDateFormat &self, const Formattable &obj, UnicodeString &append_to,
             FieldPositionIterator *pos_iter) -> UnicodeString & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.format(obj, append_to, pos_iter, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("obj"), py::arg("append_to"), py::arg("pos_iter"))
      .def(
          // [3] Format::format
          "format",
          [](const SimpleDateFormat &self, const Formattable &obj, UnicodeString &append_to) -> UnicodeString & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.format(obj, append_to, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("obj"), py::arg("append_to"))
      .def(
          // [8] DateFormat::format
          "format", py::overload_cast<UDate, UnicodeString &>(&DateFormat::format, py::const_), py::arg("date"),
          py::arg("append_to"))
      .def(
          // [9] DateFormat::format
          "format", py::overload_cast<UDate, UnicodeString &, FieldPosition &>(&DateFormat::format, py::const_),
          py::arg("date"), py::arg("append_to"), py::arg("field_position"))
      .def(
          // [10] DateFormat::format
          "format",
          [](const DateFormat &self, UDate date, UnicodeString &append_to,
             FieldPositionIterator *pos_iter) -> UnicodeString & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.format(date, append_to, pos_iter, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("date"), py::arg("append_to"), py::arg("pos_iter"));
  sdf.def("get_2digit_year_start", [](const SimpleDateFormat &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.get2DigitYearStart(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  sdf.def("get_date_format_symbols", &SimpleDateFormat::getDateFormatSymbols, py::return_value_policy::reference);
  // FIXME: Implement "const NumberFormat *icu::SimpleDateFormat::getNumberFormatForField(char16_t field)".
#if (U_ICU_VERSION_MAJOR_NUM >= 50)
  sdf.def("get_time_zone_format", &SimpleDateFormat::getTimeZoneFormat, py::return_value_policy::reference);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 50)
  sdf.def(
         // [1] SimpleDateFormat::parse
         "parse",
         py::overload_cast<const UnicodeString &, Calendar &, ParsePosition &>(&SimpleDateFormat::parse, py::const_),
         py::arg("text"), py::arg("cal"), py::arg("pos"))
      .def(
          // const char16_t *text -> const UnicodeString &text
          "parse",
          [](const SimpleDateFormat &self, const char16_t *text, Calendar &cal, ParsePosition &pos) {
            self.parse(text, cal, pos);
          },
          py::arg("text"), py::arg("cal"), py::arg("pos"))
      .def(
          // [2] DateFormat::parse
          "parse", py::overload_cast<const UnicodeString &, ParsePosition &>(&DateFormat::parse, py::const_),
          py::arg("text"), py::arg("pos"))
      .def(
          // const char16_t *text -> const UnicodeString &text
          "parse",
          [](const DateFormat &self, const char16_t *text, ParsePosition &pos) { return self.parse(text, pos); },
          py::arg("text"), py::arg("pos"))
      .def(
          // [3] DateFormat::parse
          "parse",
          [](const DateFormat &self, const UnicodeString &text) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.parse(text, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("text"))
      .def(
          // const char16_t *text -> const UnicodeString &text
          "parse",
          [](const DateFormat &self, const char16_t *text) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.parse(text, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("text"));
  sdf.def(
      "set_2digit_year_start",
      [](SimpleDateFormat &self, UDate d) {
        UErrorCode error_code = U_ZERO_ERROR;
        self.set2DigitYearStart(d, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
      },
      py::arg("d"));
#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  sdf.def(
      "set_context",
      [](SimpleDateFormat &self, UDisplayContext value) {
        UErrorCode error_code = U_ZERO_ERROR;
        self.setContext(value, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
      },
      py::arg("value"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)
  sdf.def("set_date_format_symbols", &SimpleDateFormat::setDateFormatSymbols, py::arg("new_format_symbols"));
#if (U_ICU_VERSION_MAJOR_NUM >= 50)
  sdf.def("set_time_zone_format", &SimpleDateFormat::setTimeZoneFormat, py::arg("new_time_zone_format"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 50)
  sdf.def(
      "to_localized_pattern",
      [](const SimpleDateFormat &self, UnicodeString &result) -> UnicodeString & {
        UErrorCode error_code = U_ZERO_ERROR;
        auto &string = self.toLocalizedPattern(result, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return string;
      },
      py::arg("result"));
  sdf.def("to_pattern", &SimpleDateFormat::toPattern, py::arg("result"));
}
